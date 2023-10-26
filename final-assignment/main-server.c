#include <pthread.h>
#include <sys/wait.h>
#include "shared.h"
#include "cJSON.h"
#include "os_net/os_net.h"

static pthread_mutex_t ipc_pipe_mutex; 
static os_queue_t *shared_data;

static void help_msg();
void prevent_zombie(int signum);
void *connection_mgr(void *arg);
void *data_mgr(void *arg);
void *storage_mgr(void *arg);


char *msg1 = "hello, world #1\n";
char *msg2 = "hello, world #2\n";
char *msg3 = "hello, world #3\n";

int main(int argc, char **argv)
{
    int pfds[2];
    int debug_level = 0;
    int ret;
    unsigned short port = 0;

    if(argc < 2)
        help_msg();

    /* Get option */
    {
        int c;
        while(c = getopt(argc, argv, "hdp:"), c != -1)
        {
            switch(c)
            {
                case 'h':
                    help_msg();
                    break;

                case 'd':
                    debug_level = 1;
                    nowDebug();
                    break;

                case 'p':
                    if (!optarg)
                        merror_exit("-%c needs an argument", c);
            
                    if (port = (unsigned short)atoi(optarg), port == 0)
                        merror_exit("Invalid port: %s", optarg);
                    break;
                
                default:
                    printf("wtf");
                    help_msg();
                    break;
            }
        }
    }

    if(pipe(pfds) == -1)
        merror_exit(PIPE_ERROR, errno, strerror(errno));


    mutex_init(&ipc_pipe_mutex, NULL);
    pid_t child;

    if(child = fork(), child == -1)
        merror_exit(FORK_ERROR, errno, strerror(errno));
    else if(child == 0)
    {
        /* Child */
        if(close(pfds[1]) == -1) /* Close unused write end in Child */
            merror_exit(FCLOSE_ERROR, errno, strerror(errno));

        char recv_buf[OS_MSG_SIZE];
        
        FILE *fp = fopen(GATEWAY_LOG, "a");
        if(!fp)
            merror_exit(FOPEN_ERROR, errno, strerror(errno));

        while(1)
        {   
            char *timestamp = NULL; 
            os_calloc(TIME_LENGTH, sizeof(char), timestamp);
            get_timestamp(time(NULL), timestamp);

            char *buf_to_write;
            os_malloc(OS_PIPE_BUFFER_SIZE, buf_to_write);

            ret = read(pfds[0], recv_buf, OS_MSG_SIZE);
            if(ret > 0)
            {
                sprintf(buf_to_write, "%s [%s]: %s", timestamp, "EVENT-LOG", recv_buf);
                mdebug1("%s", buf_to_write);
                fwrite(buf_to_write, strlen(buf_to_write), 1, fp); /* Write data from pipe to log file */
            }
            else
            {
                if(ret == 0)
                {
                    mdebug1("%s [%s]: Read end of Pipe", timestamp, "EVENT-LOG");
                    break;
                }
                else
                    merror_exit(FREAD_ERROR, errno, strerror(errno));
                
            }
            os_free(timestamp);
            os_free(buf_to_write);
        
        }


        fclose(fp);
        if(close(pfds[0]) == -1)
            merror_exit(FCLOSE_ERROR, errno, strerror(errno));

        mdebug1("Child process terminated.");
        _exit(OS_SUCCESS);
    }
    else 
    {
        if(close(pfds[0]) == -1) /* Close unused read end in Parent */
            merror_exit(FCLOSE_ERROR, errno, strerror(errno));

        signal(SIGCHLD, prevent_zombie);
        // write_to_pipe(&ipc_pipe_mutex, pfds[1], msg1);
        // write_to_pipe(&ipc_pipe_mutex, pfds[1], msg2);
        // write_to_pipe(&ipc_pipe_mutex, pfds[1], msg3);

        // if(close(pfds[1]) == -1) /* Child will see End of Pipe */
        //     merror_exit(FCLOSE_ERROR, errno, strerror(errno));

        shared_data = queue_init(1000);
        
        pthread_t threads[THREADS_NUM];
        void *exit_codes[THREADS_NUM];

        pthread_create(&(threads[0]), NULL, &connection_mgr, &port);
        // pthread_create(&(threads[1]), NULL, &data_mgr, NULL);

        pthread_join(threads[0], NULL);
        // pthread_join(threads[1], NULL);
        queue_free(shared_data);

    }
}

void *connection_mgr(void *arg)
{
    fd_set master;    // master file descriptor list
    fd_set read_fds;
    char ip[IPSIZE + 1];
    memset(ip, '\0', IPSIZE + 1);

    int listener, new_client, fd_max = 0;

    char buf[OS_BUFFER_SIZE];
    int bytes;

    unsigned short port = *(unsigned short *)arg;

    if (listener = OS_Bindport(port, NULL), listener <= 0)
    {
        merror(BIND_ERROR, port, errno, strerror(errno));
        exit(1);
    }

    FD_ZERO(&master);
    FD_ZERO(&read_fds);
    FD_SET(listener, &master);
    
    fd_max = listener;

    while(1)
    {
        read_fds = master;
        if(select(fd_max + 1, &read_fds, NULL, NULL, NULL) == -1)
        {
            merror_exit(SELECT_ERROR, errno, strerror(errno));
            exit(1);
        }

        for(int i = 0; i <= fd_max; i++)
        {
            if(FD_ISSET(i, &read_fds))
            {
                if(i == listener)
                {
                    /* new connection */
                    if(new_client = OS_AcceptTCP(listener, ip, IPSIZE), new_client == -1)
                        merror(ACCEPT_ERROR, strerror(errno), errno);
                    else 
                    {
                        FD_SET(new_client, &master);
                        if(new_client > fd_max)
                            fd_max = new_client;
                        
                        minfo("New connection from %s", ip);
                    }
                }
                else
                {
                    /* Hanlde current connection */
                    if(bytes = recv(i, buf, sizeof buf, 0) <= 0)
                    {
                        if(bytes == 0)
                            minfo("Socket %d hung up", i);
                        else 
                            merror(RECV_ERROR, errno, strerror(errno));

                        OS_CloseSocket(i);
                        FD_CLR(i, &master);
                    }
                    else
                    {

                        if(queue_push_ex(shared_data, buf) == -1)
                            merror("Queue is full.");
                        
                        printf("Data from client %d:'%s'\nQueue size: %d\n", i, buf, shared_data->elements);
                    }
                }
            }
        }
    }

}

void *data_mgr(void *arg)
{
    char *msg;
    char room_buf[OS_MAXSTR];
    os_malloc(OS_BUFFER_SIZE, msg);
    cJSON *msg_root = NULL;
    cJSON *root = NULL, *IDs = NULL, *room_obj = NULL;
    int senIDs[16], idx = 0, count = 1;


    if (root = json_fread("room.json", 0), !root) 
    {
        if (errno) 
            mdebug1("Couldn't load configuration file due to: %s (%d)", strerror(errno), errno);
        else 
            mdebug1("Couldn't load configuration file. Maybe format is invalid.");
    }

    room_obj = cJSON_GetObjectItem(root, "roomID");
    if(!cJSON_IsObject(room_obj))
    {
        cJSON_Delete(room_obj);
        return NULL;
    }
    
    IDs = cJSON_GetObjectItem(room_obj, "IDs");
    if(!cJSON_IsArray(IDs))
    {
        cJSON_Delete(IDs);
        return NULL;
    }

    for(int i = 0; i < cJSON_GetArraySize(IDs); i++)
    {
        cJSON *subRID = cJSON_GetArrayItem(IDs, i);
        if(cJSON_IsString(subRID))
        {
            cJSON *sensor_arr = cJSON_GetObjectItem(room_obj, subRID->valuestring);
            if (!cJSON_IsArray(sensor_arr))
            {
                cJSON_Delete(sensor_arr);
                return NULL;
            }
            
            for(int j = 0; j < cJSON_GetArraySize(sensor_arr); j++)
            {
                cJSON *subSID = cJSON_GetArrayItem(sensor_arr, j);
                if(cJSON_IsNumber(subSID))
                    senIDs[idx++] = subSID->valueint;
                else 
                    cJSON_Delete(subSID);
            }
        }
        else
            cJSON_Delete(subRID);
    }

    sort(senIDs, 0, 15);

    while(1)
    {
        msg = queue_pop_ex(shared_data);
        // printf("Message from queue: %s\n", msg);
        msg_root = cJSON_Parse(msg);
        if(!cJSON_IsObject(msg_root))
            continue;
        cJSON *subSID = cJSON_GetObjectItem(msg_root, "sensorID");
        if(cJSON_IsNumber(subSID))
            printf("%d\n", subSID->valueint);
        else 
            cJSON_Delete(subSID);

    }

    cJSON_Delete(room_obj);
    cJSON_Delete(msg_root);
    os_free(msg);
}
void *storage_mgr(void *arg)
{

}

static void help_msg()
{
    print_out("%s - %s (%s)", __ass_name, __author, __contact);
    print_out("  Server: -[hdf] [-p port]");
    print_out("    -h          This help message.");
    print_out("    -d          Debug mode. Use this parameter multiple times to increase the debug level.");
    print_out("    -p <port>   Manager port.");
    exit(1);

}

void prevent_zombie(int signum)
{
    wait(NULL);
    minfo("Child procsess termination\n");

}
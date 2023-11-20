#include <pthread.h>
#include <sys/wait.h>
#include "shared.h"
#include "cJSON.h"
#include "os_net/os_net.h"

static pthread_mutex_t ipc_pipe_mutex; 
static pthread_mutex_t cnt_ex; 
static os_list *shared_data;
int pfds[2], cnt = 0;

static void help_msg();
void prevent_zombie(int signum);
void *connection_mgr(void *arg);
void *data_mgr(void *arg);
void *storage_mgr(void *arg);

int main(int argc, char **argv)
{
    int debug_level = 0;
    int ret = 0;
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
                    use_debug();
                    break;

                case 'p':
                    if (!optarg)
                        merror_exit("-%c needs an argument", c);
            
                    if (port = (unsigned short)atoi(optarg), port == 0)
                        merror_exit("Invalid port: %s", optarg);
                    break;
                
                default:
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
            merror_exit(CLOSE_ERROR, errno, strerror(errno));

        char recv_buf[OS_MSG_SIZE];
        
        int fd = open(GATEWAY_LOG, O_WRONLY);
        if(!fd)
            merror_exit(OPEN_ERROR, errno, strerror(errno));   

        if(lseek(fd, 0, SEEK_END) < 0)
            merror(SEEK_ERROR, errno, strerror(errno));

        size_t bytes = -1;

        while(ret = read(pfds[0], recv_buf, OS_MSG_SIZE), ret > 0)
        {
            // printf("Buf from pipe: %s\nlen: %ld\n", recv_buf, strlen(recv_buf)); 
            if(bytes = write(fd, recv_buf, strlen(recv_buf)), bytes <= 0)
                merror(WRITE_ERROR, errno, strerror(errno));
            // else 
            //     mdebug("Bytes writed: %ld", bytes);

            fsync(fd);
        }

        if(ret == 0)
            mdebug("Read end of Pipe");
        else
            merror_exit(READ_ERROR, errno, strerror(errno));
    
        close(fd);
        if(close(pfds[0]) == -1)
            merror_exit(CLOSE_ERROR, errno, strerror(errno));

        mdebug("Child process terminated.");
        _exit(0);
    }
    else 
    {
        if(close(pfds[0]) == -1) /* Close unused read end in Parent */
            merror_exit(CLOSE_ERROR, errno, strerror(errno));

        signal(SIGCHLD, prevent_zombie);
        // if(close(pfds[1]) == -1) /* Child will see End of Pipe */
        //     merror_exit(CLOSE_ERROR, errno, strerror(errno));

        shared_data = list_create(100);
        mutex_init(&cnt_ex, NULL);
    
        int check_read1= 0, check_read2 = 1;
        
        pthread_t threads[THREADS_NUM];

        pthread_create(&(threads[0]), NULL, &connection_mgr, &port);
        pthread_create(&(threads[1]), NULL, &data_mgr, &check_read1);
        pthread_create(&(threads[2]), NULL, &storage_mgr, &check_read2);

        for( int i = 0; i < THREADS_NUM; i++ ) 
            pthread_join(threads[i], NULL);

        list_destroy(shared_data);
    }
}

void *connection_mgr(void *arg)
{
    mdebug("Connection manager started.");
    fd_set master;    // master file descriptor list
    fd_set read_fds;
    char ip[IPSIZE + 1];
    memset(ip, '\0', IPSIZE + 1);
    int listener, new_client, fd_max = 0, bytes;
    char buf[OS_BUFFER_SIZE];
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
            merror(SELECT_ERROR, errno, strerror(errno));
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
                            minfo(SENSOR_CONNECTION_CLOSE, i);
                        else 
                            merror(RECV_ERROR, errno, strerror(errno));

                        OS_CloseSocket(i);
                        FD_CLR(i, &master);
                    }
                    else
                    {
                        if(list_push(shared_data, buf))
                            mdebug("Can not push data to shared data");
                        
                        // printf("Data from client %d:'%s'\nList size: %d\n", i, buf, shared_data->cur_size);
                    }
                }
            }
        }
    }

}

void *data_mgr(void *arg)
{
    mdebug("Data manager started.");
    char *msg, *timestamp;
    char old_msg[OS_BUFFER_SIZE], room_buf[OS_MAXSTR];
    memset(old_msg, '\0', sizeof(old_msg));
    os_malloc(OS_BUFFER_SIZE, msg);
    os_malloc(TIME_LENGTH, timestamp);

    cJSON *msg_root = NULL;
    cJSON *root = NULL, *IDs = NULL, *room_obj = NULL;
    int senIDs[5], temp_avg = -1, check = 0, read = *(int *)arg;
    int val[6][6] = {0};
    for(int i = 1; i <= 5; i++)
        val[i][0] = 1;

    if (root = json_fread("room.json", 0), !root) 
    {
        if (errno) 
            mdebug("Couldn't load configuration file due to: %s (%d)", strerror(errno), errno);
        else 
            mdebug("Couldn't load configuration file. Maybe format is invalid.");
    }

    room_obj = cJSON_GetObjectItem(root, "roomIDs");
    if(!cJSON_IsArray(room_obj))
    {
        cJSON_Delete(room_obj);
        return NULL;
    }

    for(int i = 0; i < cJSON_GetArraySize(room_obj); i++)
    {
        cJSON *subRID = cJSON_GetArrayItem(room_obj, i);
        if(cJSON_IsObject(subRID))
        {
            cJSON *subSID = cJSON_GetObjectItem(subRID, "sensorID");
            if(cJSON_IsNumber(subSID))
                senIDs[i] = subSID->valueint;
            else 
                cJSON_Delete(subSID);
        }   
        else 
            cJSON_Delete(subRID);
    }

    while(1)
    {
        mutex_lock(&cnt_ex);
        if(msg = list_get_data_first_node(shared_data), msg != NULL)
        {
            if(strcmp(msg, old_msg))
            {
                strcpy(old_msg, msg);
                cnt++;
            }
            else 
            {
                mutex_unlock(&cnt_ex);
                continue;
            }
        }
        if(cnt == 2)
        {
            cnt = 0;
            list_remove_first_node(shared_data);
        }

        mutex_unlock(&cnt_ex);


        printf("At Data Manager. Message: %s\n", msg);
        msg_root = cJSON_Parse(msg);
        if(!cJSON_IsObject(msg_root))
            continue;
        

        cJSON *SID = cJSON_GetObjectItem(msg_root, "sensorID");
        if(cJSON_IsNumber(SID))
        {
            cJSON *ts = cJSON_GetObjectItem(msg_root, "timestamp");
            if(cJSON_IsString(ts))
                strcpy(timestamp, ts->valuestring);
            else 
                cJSON_Delete(ts);

            if(search(senIDs, SID->valueint, 0, 5))
            {
                cJSON *temp = cJSON_GetObjectItem(msg_root, "temperature");
                if(cJSON_IsNumber(temp))
                {
                    val[SID->valueint][val[SID->valueint][0]++] = temp->valueint;
                    
                    if(check)
                        temp_avg = avg(val[SID->valueint]);
                    
                    if(val[SID->valueint][0] > 5)
                    {
                        val[SID->valueint][0] = 1;
                        if(!check)
                        {
                            check = 1;
                            temp_avg = avg(val[SID->valueint]);
                        }
                    }

                    if(temp_avg <= 0)
                        continue;
                    
                    if(temp_avg >= 26)
                    {
                        minfo(SENSOR_HOT, timestamp, SID->valueint, temp_avg);
                        write_to_pipe(&ipc_pipe_mutex, pfds[1], SENSOR_HOT, timestamp, SID->valueint, temp_avg);
                    }
                    else if(temp_avg <= 24)
                    {
                        minfo(SENSOR_COLD, timestamp, SID->valueint, temp_avg);
                        write_to_pipe(&ipc_pipe_mutex, pfds[1], SENSOR_COLD, timestamp, SID->valueint, temp_avg);
                    }
                    else 
                    {
                        minfo("Temperature at sensor '%d': %d", SID->valueint, temp_avg);
                        write_to_pipe(&ipc_pipe_mutex, pfds[1], SENSOR_TEMP, timestamp, SID->valueint, temp_avg);
                    }
                }
                else 
                    cJSON_Delete(temp);
            }
            else 
                write_to_pipe(&ipc_pipe_mutex, pfds[1], SENSOR_INVALID, timestamp, SID->valueint);
        }
        else
            cJSON_Delete(SID);
        
    }

    cJSON_Delete(room_obj);
    cJSON_Delete(msg_root);
    os_free(timestamp);
    os_free(msg);

}
void *storage_mgr(void *arg)
{
    mdebug("Storage manager started.");
    char *msg;
    char old_msg[OS_BUFFER_SIZE];
    memset(old_msg, '\0', sizeof(old_msg));
    os_malloc(OS_BUFFER_SIZE, msg);
    int read = *(int *)arg;

    while(1)
    {
        mutex_lock(&cnt_ex);
        if(msg = list_get_data_first_node(shared_data), msg != NULL)
        {
            if(strcmp(msg, old_msg))
            {
                strcpy(old_msg, msg);
                cnt++;
                printf("At Storage Manager. Message: %s\n", msg);
            }
            else 
            {
                mutex_unlock(&cnt_ex);
                continue;
            }
        }
        if(cnt == 2)
        {
            cnt = 0;
            list_remove_first_node(shared_data);
        }
        mutex_unlock(&cnt_ex);
        // sleep(2);
    }

}

static void help_msg()
{
    print_out("%s - %s (%s)", __ass_name, __author, __contact);
    print_out("  Server: -[hdf] [-p port]");
    print_out("    -h          This help message.");
    print_out("    -d          Debug mode.");
    print_out("    -p <port>   Manager port.");
    exit(1);

}

void prevent_zombie(int signum)
{
    wait(NULL);
    minfo("Child procsess termination\n");

}
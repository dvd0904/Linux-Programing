#include <pthread.h>
#include <sys/wait.h>
#include "shared.h"
#include "cJSON.h"
#include "os_net/os_net.h"
#include "fdb/fdb.h"

static pthread_mutex_t ipc_pipe_mutex; 
static pthread_rwlock_t sen_lock;
static os_list *shared_data;
int pfds[2], cnt = 0, *senIDs;


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
            if(bytes = write(fd, recv_buf, strlen(recv_buf)), bytes <= 0)
                merror(WRITE_ERROR, errno, strerror(errno));

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
        rwlock_init(&sen_lock, NULL);
        senIDs = read_room();
        shared_data = list_create(100);
    
        int check_read1= 0, check_read2 = 1;
        
        pthread_t threads[THREADS_NUM];

        pthread_create(&(threads[0]), NULL, &connection_mgr, &port);
        pthread_create(&(threads[1]), NULL, &data_mgr, &check_read1);
        pthread_create(&(threads[2]), NULL, &storage_mgr, &check_read2);

        for( int i = 0; i < THREADS_NUM; i++ ) 
            pthread_join(threads[i], NULL);

        list_destroy(shared_data);
        rwlock_destroy(&sen_lock);
        os_free(senIDs);
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

    if (listener = OS_BindPort(port), listener <= 0)
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
                        
                    }
                }
            }
        }
    }

}

void *data_mgr(void *arg)
{
    mdebug("Data manager started.");
    char *msg = NULL;
    char old_msg[OS_BUFFER_SIZE];
    memset(old_msg, '\0', sizeof(old_msg));

    msg_t *parsed = NULL;
    int temp_avg[5], check[5] = {0}, read = *(int *)arg;
    int val[6][6] = {0};
    for(int i = 1; i <= 5; i++)
    {
        val[i][0] = 1;
        temp_avg[i - 1] = -1;
    }

    while(1)
    {
        mutex_lock(&shared_data->cnt_mutex);
        if(msg = list_get_data_first_node(shared_data), msg != NULL)
        {
            if(strcmp(msg, old_msg))
            {
                strcpy(old_msg, msg);
                cnt++;
            }
            else 
            {
                mutex_unlock(&shared_data->cnt_mutex);
                continue;
            }
        }
        if(cnt == 2)
        {
            cnt = 0;
            list_remove_first_node(shared_data);
        }
        mutex_unlock(&shared_data->cnt_mutex);

        // printf("At Data Manager. Message: %s\n", msg);
        rwlock_rdlock(&sen_lock);
        parsed = msg_parse(msg, senIDs);
        rwlock_unlock(&sen_lock);

        if(parsed != NULL)
        {   
            val[parsed->senID][val[parsed->senID][0]++] = parsed->temp;
            if(check[parsed->senID - 1])
                temp_avg[parsed->senID - 1] = avg(val[parsed->senID]);
            
            
            if(val[parsed->senID][0] > 5)
            {
                val[parsed->senID][0] = 1;
                if(!check[parsed->senID - 1])
                {
                    check[parsed->senID - 1] = 1;
                    temp_avg[parsed->senID - 1] = avg(val[parsed->senID]);
                }
            }


            if(temp_avg[parsed->senID - 1] <= 0)
                continue;
            
            if(temp_avg[parsed->senID - 1] >= 26)
            {
                minfo(SENSOR_HOT, parsed->ts, parsed->senID, temp_avg[parsed->senID - 1]);
                write_to_pipe(&ipc_pipe_mutex, pfds[1], SENSOR_HOT"\n", parsed->ts, parsed->senID, temp_avg[parsed->senID - 1]);
            }
            else if(temp_avg[parsed->senID - 1] <= 24)
            {
                minfo(SENSOR_COLD, parsed->ts, parsed->senID, temp_avg[parsed->senID - 1]);
                write_to_pipe(&ipc_pipe_mutex, pfds[1], SENSOR_COLD"\n", parsed->ts, parsed->senID, temp_avg[parsed->senID - 1]);
            }
            else 
            {
                minfo("Temperature at sensor '%d': %d", parsed->senID, temp_avg[parsed->senID - 1]);
                write_to_pipe(&ipc_pipe_mutex, pfds[1], SENSOR_TEMP"\n", parsed->ts, parsed->senID, temp_avg[parsed->senID - 1]);
            }
        }
        os_free(parsed);
    }

    os_free(senIDs);
    os_free(msg);

}

void *storage_mgr(void *arg)
{
    mdebug("Storage manager started.");
    char *msg = NULL;

    char old_msg[OS_BUFFER_SIZE];
    memset(old_msg, '\0', sizeof(old_msg));
    
    int read = *(int *)arg;
    msg_t *parsed = NULL;
    fdb_t *fdb = fdb_open_sensor_db();
    if(fdb != NULL)
        minfo(DB_NEW_CONNECTION);
    else 
        merror(DB_UNABLE_CONNECTION);

    while(1)
    {
        mutex_lock(&shared_data->cnt_mutex);
        if(msg = list_get_data_first_node(shared_data), msg != NULL)
        {
            if(strcmp(msg, old_msg))
            {
                strcpy(old_msg, msg);
                cnt++;
            }
            else 
            {
                mutex_unlock(&shared_data->cnt_mutex);
                continue;
            }
        }
        if(cnt == 2)
        {
            cnt = 0;
            list_remove_first_node(shared_data);
        }
        mutex_unlock(&shared_data->cnt_mutex);

        // printf("At Storage Manager. Message: %s\n", msg);
        rwlock_rdlock(&sen_lock);
        parsed = msg_parse(msg, senIDs);
        rwlock_unlock(&sen_lock);

        if(parsed != NULL)
        {
            if(!fdb_data_insert(fdb, parsed->senID, parsed->temp, parsed->ts))
                mdebug(DB_INSERT);

            os_free(parsed);
        }
    }

    fdb_destroy(fdb);
    os_free(senIDs);
    os_free(msg);
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
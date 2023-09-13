#include <pthread.h>
#include <sys/wait.h>
#include "shared.h"
#include "os_net/os_net.h"
#include "cJSON.h"

volatile int running = 1;

static tcpsock_t * server_sock;
static dbl_list_t *socket_list = NULL;
static struct pollfd *poll_fds;
static w_queue_t *share_data_queue = NULL;

/* Signal handler */
static void handler(int signum);

/* Thread for manage new client */
static void *run_connection_manager(void *arg);

/* Thread for manage data */
static void *run_data_manager(void *arg);

/* Thread for storage data*/
static void *run_storage_manager(void *arg);


static void help_msg()
{
    print_out("%s - %s (%s)", __ass_name, __author, __contact);
    print_out("  %s: -[hdf] [-p port]", ARGV0);
    print_out("    -h          This help message.");
    print_out("    -d          Debug mode. Use this parameter multiple times to increase the debug level.");
    print_out("    -p <port>   Manager port. Default: %d.", DEFAULT_PORT);
    exit(1);

}

int main(int argc, char **argv)
{
    int debug_level = 0;
    int run_foreground = 0;
    int status;
    unsigned short port = 0;
    /* Get option */
    {
        int c;

        while(c = getopt(argc, argv, "hdfp:"), c != -1)
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

                case 'f':
                    run_foreground = 1;
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



    pthread_t threads_id[3];

    share_data_queue = queue_init(AUTH_POOL);

    unsigned short arg0 = port;

    pthread_create(&threads_id[0], NULL, &run_connection_manager, &arg0);


    for(int i = 0; i < 3; i++)
        pthread_join(threads_id[i], NULL);

    minfo("Exiting...");
}

/* Thread for remote server */
static void *run_connection_manager(void *arg)
{
    server_sock->port = *((unsigned short *)arg);
    if(server_sock->port < MIN_PORT || server_sock->port > MAX_PORT)
        merror_exit("[%s]: Invalid Port", CONNECTION_MANAGER);

    os_malloc(sizeof(struct pollfd), poll_fds);

    if (server_sock->sd = OS_Bindporttcp(server_sock->port, NULL), server_sock->sd <= 0)
    {
        merror(BIND_ERROR, server_sock->port, errno, strerror(errno));
        exit(1);
    }

    minfo("[%s]: Listening for client...", CONNECTION_MANAGER);

    OS_GetSocketSd(&(server_sock->sd), &(poll_fds[0].fd));
    poll_fds[0].revents = POLLIN;

    tcpsock_el_t *client;
    tcpsock_el_t dummy;
    tcpsock_el_t tmp;
    dbl_list_t *node;

    int conn_cnt = 0;
    int poll_res, tcp_connection_res;

    while((poll_res = poll(poll_fds, (conn_cnt + 1), TIMEOUT*1000)) || conn_cnt)
    {
        if(poll_res == -1) break;

        if((poll_fds[0].revents & POLLIN) && conn_cnt < BACKLOG)
        {
            mdebug1("[%s]: Incoming client connection\n", CONNECTION_MANAGER);

            os_malloc(sizeof(tcpsock_el_t), client);
            
            if(tcp_connection_res = OS_AcceptTCP(server_sock, &(client->sock_ptr), IPSIZE) < 0)
                merror("[%s]: failed to accept new connection", CONNECTION_MANAGER);
            else 
            {
                conn_cnt++;
                os_realloc(poll_fds, sizeof(struct pollfd) * (conn_cnt + 1), poll_fds);

                OS_GetSocketSd(&(client->sock_ptr->sd), &(poll_fds[conn_cnt].fd));
                client->sd = poll_fds[conn_cnt].fd;
                client->last_active = get_timestamp(time(NULL));
                client->sensor = 0;
                poll_fds[conn_cnt].events = POLLIN | POLLHUP;

                dbl_list_insert_end(&socket_list, client);

                minfo("[%s]: new connection received", CONNECTION_MANAGER);
            }

        }

        poll_res--;

    }

    for(int i = 1; i < (conn_cnt = 1) && poll_res > 0; i++)
    {
        dummy.sd = poll_fds[i].fd;
        node = dbl_list_get_node(socket_list, &dummy);
    }


    return NULL;
}

/* Thread for dispatch data */


/* Thread for manage data */
static void *run_data_manager(void *arg)
{

}

/* Thread for storage data*/
static void *run_storage_manager(void *arg)
{

}

/* Signal handler */
void handler(int signum)
{
    switch (signum)
    {
    case SIGHUP:
    case SIGINT:
    case SIGTERM:
        minfo(SIGNAL_RECV, signum, strsignal(signum));
        running = 0;
        break;
    default:
        merror("unknown signal (%d)", signum);
    }
}

void authd_sigblock()
{
    sigset_t sigset;
    sigemptyset(&sigset);
    sigaddset(&sigset, SIGTERM);
    sigaddset(&sigset, SIGHUP);
    sigaddset(&sigset, SIGINT);
    pthread_sigmask(SIG_BLOCK, &sigset, NULL);
}


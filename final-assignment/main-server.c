#include <pthread.h>
#include <sys/wait.h>
#include "shared.h"
#include "cJSON.h"
#include "os_auth/auth.h"

authd_config_t config;
static int sock = -1;
volatile int running = 1;

/* client queue */
static w_queue_t *client_queue = NULL;

/* Signal handler */
static void handler(int signum);

/* Thread for manage new client */
static void *run_connection_manager(void *arg);

/* Thread for dispatcher client */
static void *run_dispatcher_manager(void *arg);

/* Thread for manage data */
static void *run_data_manager(void *arg);

/* Thread for storage data*/
static void *run_storage_manager(void *arg);

pthread_mutex_t mutex_keys = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t cond_pending = PTHREAD_COND_INITIALIZER;

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

    pthread_t thread_connection_manager = 0;
    pthread_t thread_data_managemer = 0;
    pthread_t thread_storage_manager = 0;
    pthread_t thread_dispatcher_manager = 0;

    /* Get option */
    {
        int c;
        unsigned short port = 0;

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

        if(port)
            config.port = port;

        if (run_foreground)
            config.flags.disabled = 0;
    }

    if (!run_foreground)
    {

    }

    /* Connect via TCP */
    if (sock = OS_Bindporttcp(config.port, NULL), sock <= 0) /* bind + listen*/
    {
        merror(BIND_ERROR, config.port, errno, strerror(errno));
        exit(1);
    }

    minfo("Accepting connections on port %hu.", config.port);

    client_queue = queue_init(AUTH_POOL);

    /* accept */
    if (status = pthread_create(&thread_connection_manager, NULL, (void *)&run_connection_manager, NULL), status != 0)
    {
        merror("Couldn't create thread: %s", strerror(status));
        return EXIT_FAILURE;
    }

    if (status = pthread_create(&thread_dispatcher_manager, NULL, (void *)&run_dispatcher_manager, NULL), status != 0)
    {
        merror("Couldn't create thread: %s", strerror(status));
        return EXIT_FAILURE;
    }
    



    pthread_join(thread_connection_manager, NULL);
    pthread_join(thread_dispatcher_manager, NULL);

    queue_free(client_queue);
    minfo("Exiting...");
}

/* Thread for remote server */
static void *run_connection_manager(void *arg)
{
    int client_sock = 0;
    struct sockaddr_in _new_client;
    socklen_t _new_client_length;
    fd_set fdset;
    struct timeval timeout;

    authd_sigblock();

    if (config.timeout_sec || config.timeout_usec)
    {
        minfo("Setting network timeout to %.6f sec.", config.timeout_sec + config.timeout_usec / 1000000.);
    }
    else
    {
        mdebug1("Network timeout is disabled.");
    }

    mdebug1("Remote server ready.");

    while (running)
    {
        memset(&_new_client, 0, sizeof(_new_client));
        _new_client_length = sizeof(_new_client);

        // Wait for socket
        FD_ZERO(&fdset);
        FD_SET(sock, &fdset);
        timeout.tv_sec = 1;
        timeout.tv_usec = 0;

        switch (select(sock + 1, &fdset, NULL, NULL, &timeout))
        {
        case -1:
            if (errno != EINTR)
            {
                merror_exit("at main(): select(): %s", strerror(errno));
            }
            continue;
        case 0:
            continue;
        }

        if ((client_sock = accept(sock, (struct sockaddr *)&_new_client, &_new_client_length)) > 0)
        {
            if (config.timeout_sec || config.timeout_usec)
            {
                if (OS_SetRecvTimeout(client_sock, config.timeout_sec, config.timeout_usec) < 0)
                {
                    static int reported = 0;

                    if (!reported)
                    {
                        int error = errno;
                        merror("Could not set timeout to network socket: %s (%d)", strerror(error), error);
                        reported = 1;
                    }
                }
            }
            struct client *new_client;
            os_malloc(sizeof(struct client), new_client);
            new_client->socket = client_sock;
            new_client->addr = _new_client.sin_addr;

            if (queue_push_ex(client_queue, new_client) == -1)
            {
                merror("Too many connections. Rejecting.");
                close(client_sock);
            }
        }
        else if ((errno == EBADF && running) || (errno != EBADF && errno != EINTR))
            merror("at main(): accept(): %s", strerror(errno));
    }

    mdebug1("Remote server thread finished");

    close(sock);
    return NULL;
}

/* Thread for dispatch data */
static void *run_dispatcher_manager(void *arg)
{
    char ip[IPSIZE + 1];
    int ret;
    char *buf = NULL;
    char response[2048];
    response[2047] = '\0';

    authd_sigblock();

    memset(ip, '\0', IPSIZE + 1);

    mdebug1("Dispatch thread ready.");

    while(running)
    {
        const struct timespec timeout = {.tv_sec = time(NULL) + 1};
        struct client *client = queue_pop_ex_timedwait(client_queue, &timeout);

        if (queue_push_ex(client_queue, client) == -1)
        {
            merror("Too many connections. Rejecting.");
            close(client->socket);
        }

        if (!client)
        {
            continue;
        }

        strncpy(ip, inet_ntoa(client->addr), IPSIZE - 1);
        minfo("New connection from %s", ip);

        char *buf = OS_RecvTCP(client->socket, OS_SIZE_1024);
        minfo("Buf: %s", buf);
    }

}

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


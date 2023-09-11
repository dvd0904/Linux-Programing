#include <pthread.h>
#include <sys/wait.h>
#include "shared.h"
#include "cJSON.h"
#include "os_auth/auth.h"

static int sock = -1;
volatile int running = 1;

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
    unsigned short port = *((int *)arg);
    if(port < MIN_PORT || port > MAX_PORT)
        merror_exit("[CONNECTION MANAGER]: Invalid Port");

    


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


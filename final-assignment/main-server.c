#include "shared.h"
#include "cJSON.h"
#include <pthread.h>
#include <sys/wait.h>

config_t config;

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

}



#include <pthread.h>
#include <sys/wait.h>
#include "shared.h"
#include "cJSON.h"
#include "os_net/os_net.h"

#define PORT "2023" // the port client will be connecting to 

static void help_msg();

int main(int argc, char *argv[])
{
	int temp;
	int senID, debug_level = 0, server_fd, ret;
	unsigned short port = 0;
	const char *ip = NULL;
	char *msg = NULL;

	if(argc < 2)
		help_msg();

	{
		int c;
		while(c = getopt(argc, argv, "hdp:a:i:"), c != -1)
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
					{
                        merror("-%c needs an argument", c);
						printf("????");
						help_msg();
					}
            		port = (unsigned short)atoi(optarg);
                    if (port < 1024 || port > 65536)
                        merror_exit("Invalid port: %s", optarg);
                    break;
				
				case 'a':
					if (!optarg)
					{
                        merror("-%c needs an argument", c);
						help_msg();
					}
					ip = optarg;
					break;
				case 'i':
					if (!optarg)
					{
                        merror("-%c needs an argument", c);
						help_msg();
					}
					senID = atoi(optarg); 
					break;
                default:
                    help_msg();
                    break;
            }
        }
	}

	if(server_fd = OS_Connect(port, ip), server_fd < 0)
		merror_exit(CONN_ERROR, ip, port, strerror(errno), errno);
	
	minfo("Connect to server successfully.");
	os_malloc(OS_BUFFER_SIZE, msg);

	srand(time(0)); // Use current time as seed for random generator

	while(1)
	{
		temp = MIN_TEMP + rand() % (MAX_TEMP - MIN_TEMP + 1);
		getMsg(&msg, senID, temp);
		if(ret = OS_SendTCP(server_fd, msg), ret < 0)
			merror(SEND_ERROR, errno, strerror(errno));
		printf("Msg send to server: %s\n", msg);
		sleep(SLEEP);
	}

	os_free(msg);
	
	return 0;
}

static void help_msg()
{
    print_out("%s - %s (%s)", __ass_name, __author, __contact);
    print_out("  Client: -[hd] [-p port] [-i sensorID] [-a IP]");
    print_out("    -h          		This help message.");
    print_out("    -d          		Debug mode.");
    print_out("    -p <port>   		Manager port.");
	print_out("    -a <ip>			Manager IP.");
	print_out("    -i <sensorID>		Sensor ID.");
    exit(1);

}

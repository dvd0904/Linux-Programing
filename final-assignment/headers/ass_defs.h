#ifndef OS_HEADERS
#define OS_HEADERS



/* Size limit control */
#define OS_SIZE_1048576             1048576
#define OS_SIZE_65536               65536
#define OS_SIZE_61440               61440
#define OS_SIZE_20480               20480
#define OS_SIZE_8192                8192
#define OS_SIZE_6144                6144
#define OS_SIZE_4096                4096
#define OS_SIZE_2048                2048
#define OS_SIZE_1024                1024
#define OS_SIZE_512                 512
#define OS_SIZE_256                 256
#define OS_SIZE_128                 128
#define OS_SIZE_64                  64
#define OS_SIZE_32                  32
#define OS_SIZE_16                  16


/* Level of log messages */

#define LOGLEVEL_CRITICAL           4
#define LOGLEVEL_ERROR              3
#define LOGLEVEL_WARNING            2
#define LOGLEVEL_INFO               1
#define LOGLEVEL_DEBUG              0


#define OS_MAXSTR                   OS_SIZE_65536       /* Size for logs, sockets, etc      */
#define OS_BUFFER_SIZE              OS_SIZE_2048        /* Size of general buffers          */
#define OS_FLSIZE                   OS_SIZE_256         /* Maximum file size                */
#define OS_HEADER_SIZE              OS_SIZE_128         /* Maximum header size              */
#define OS_LOG_HEADER               OS_SIZE_256         /* Maximum log header size          */
#define IPSIZE                      INET6_ADDRSTRLEN    /* IP Address size                  */
#define AUTH_POOL                   1000                /* Max number of connections        */
#define BACKLOG                     128                 /* Socket input queue length        */
#define MAX_EVENTS                  1024                /* Max number of epoll events       */
#define EPOLL_MILLIS                -1                  /* Epoll wait time                  */
#define MAX_TAG_COUNTER             256                 /* Max retrying counter             */
#define SOCK_RECV_TIME0             300                 /* Socket receiving timeout (s)     */
#define MIN_ORDER_SIZE              32                  /* Minimum size of orders array     */
#define KEEPALIVE_SIZE              700                 /* Random keepalive string size     */
#define MAX_DYN_STR                 4194304             /* Max message size received 4MiB   */
#define DATE_LENGTH                 64                  /* Format date time %D %T           */

/* Global name */

#define __ass_name                  "Final Assignment"
#define __author                    "Nguyen Dinh Dai"
#define __contact                   "dinhdai0904@gmail.com"

#endif /* OS_HEADERS */
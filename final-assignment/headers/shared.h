#ifndef SHARED_H
#define SHARED_H

/* Global headers */
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <sys/param.h>
#include <stdint.h>
#include <inttypes.h>
#include <assert.h>


#include <netdb.h>
#include <netinet/in.h>
#include <netinet/tcp.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/un.h>


#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <fcntl.h>
#include <dirent.h>
#include <ctype.h>
#include <signal.h>
#include <stdbool.h>
#include <pthread.h>


#include <time.h>
#include <errno.h>
#include <libgen.h>

#include "ass_defs.h"
#include "ass_debug.h"
#include "ass_time.h"
#include "ass_pthreads.h"
#include "ass_file.h"
#include "ass_err.h"
#include "ass_config.h"


#include "messages/debug_messages.h"
#include "messages/error_messages.h"


#define os_calloc(x,y,z) ((z = (__typeof__(z)) calloc(x,y)))?(void)1:merror_exit(MEM_ERROR, errno, strerror(errno))

#define os_malloc(x,y) ((y = (__typeof__(y)) malloc(x)))?(void)1:merror_exit(MEM_ERROR, errno, strerror(errno))

#define os_free(x) if(x){free(x);x=NULL;}

#define os_realloc(x,y,z) ((z = (__typeof__(z))realloc(x,y)))?(void)1:merror_exit(MEM_ERROR, errno, strerror(errno))

#endif
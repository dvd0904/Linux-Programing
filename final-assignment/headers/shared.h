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

#include "mahdefs.h"
#include "mahdebug.h"
#include "mahtime.h"
#include "mahpthreads.h"
#include "mahfile.h"

#include "messages/debug_messages.h"
#include "messages/error_messages.h"


#define os_calloc(x,y,z) ((z = (__typeof__(z)) calloc(x,y)))?(void)1:printf(MEM_ERROR, errno, strerror(errno))

#endif
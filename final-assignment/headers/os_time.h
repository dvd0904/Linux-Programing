#ifndef OSTIME_H 
#define OSTIME_H 

#define TIME_LENGTH     OS_SIZE_128

#include <time.h>


char *get_timestamp(const time_t time);

#endif
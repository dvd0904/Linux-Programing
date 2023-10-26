#ifndef OSTIME_H 
#define OSTIME_H 

#define TIME_LENGTH     OS_SIZE_128

#include <time.h>


void get_timestamp(time_t time, char *timestamp);

#endif
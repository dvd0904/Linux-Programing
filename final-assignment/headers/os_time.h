#ifndef OSTIME_H 
#define OSTIME_H 

#define TIME_LENGTH     OS_SIZE_64

#include <time.h>

/**
 * @brief Get current timestamp
 * @param time 
 * @return a string represent for current time with format "yyyy/mm/dd h:m:s"
*/
char *get_timestamp(time_t time);

#endif
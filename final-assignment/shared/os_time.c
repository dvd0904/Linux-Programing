#include "shared.h"


void get_timestamp(time_t time, char *timestamp) 
{
    struct tm localtm;
    localtime_r(&time, &localtm);

    snprintf(timestamp, TIME_LENGTH, "%d/%02d/%02d %02d:%02d:%02d",
            localtm.tm_year + 1900, localtm.tm_mon + 1,
            localtm.tm_mday, localtm.tm_hour, localtm.tm_min, localtm.tm_sec);
}


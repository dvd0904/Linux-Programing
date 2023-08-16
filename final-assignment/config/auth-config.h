#ifndef AUTH_CONFIG_H
#define AUTH_CONFIG_H


typedef struct authd_flags_t 
{
    unsigned short disabled:3;
} authd_flags_t;

typedef struct authd_config_t 
{
    unsigned short port;
    authd_flags_t flags;
    long timeout_sec;
    long timeout_usec;
} authd_config_t;




#endif
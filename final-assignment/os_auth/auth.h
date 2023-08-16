#ifndef AUTHD_H
#define AUTHD_H

#include "shared.h"
#include "os_net/os_net.h"
#include "config/auth-config.h"

#ifndef ARGV0
#define ARGV0 "ass-server"
#endif

#define DEFAULT_PORT 6543

struct client {
    int socket;
    struct in_addr addr;
};


// Block signals
void authd_sigblock();


extern volatile int write_pending;
extern volatile int running;
extern pthread_mutex_t mutex_keys;
extern pthread_cond_t cond_pending;
extern authd_config_t config;


#endif
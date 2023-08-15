#ifndef ASS_CONFIG_G
#define ASS_CONFIG_G



#ifndef ARGV0
#define ARGV0 "ass-server"
#endif

#define DEFAULT_PORT 6543

typedef struct config_flags_t {
    unsigned short disabled:3;
} config_flags_t;

typedef struct config_t {
    unsigned short port;
    config_flags_t flags;
} config_t;






#endif
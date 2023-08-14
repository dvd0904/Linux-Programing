#ifndef DEBUG_H
#define DEBUG_H

#include <stdarg.h>


#ifndef LOGFILE

#define LOGFILE         "logs/gateway.log"
#define LOGJSONFILE     "logs/gateway.json"
#define _PRINTF_FORMAT  printf

#endif


#define minfo(msg, ...) _minfo(__FILE__, __LINE__, __func__, msg, ##__VA_ARGS__)


void _minfo(const char * file, int line, const char * func, const char *msg, ...) __attribute__((format(printf, 4, 5))) __attribute__((nonnull));


/**
 * @brief Logging module initializer
 */
void logging_init(void);

#endif /* DEBUG_H */
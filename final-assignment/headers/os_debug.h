#ifndef DEBUG_H
#define DEBUG_H

#include <stdarg.h>


#ifndef LOGFILE

#define LOGFILE         "logs/debug.log"
#define GATEWAY_LOG      "logs/gateway.log"
#define _PRINTF_FORMAT  printf

#endif

#define mdebug(msg, ...) _mdebug(__FILE__, __LINE__, __func__, msg, ##__VA_ARGS__)
#define minfo(msg, ...) _minfo(__FILE__, __LINE__, __func__, msg, ##__VA_ARGS__)
#define mwarn(msg, ...) _mwarn(__FILE__, __LINE__, __func__, msg, ##__VA_ARGS__)
#define merror(msg, ...) _merror(__FILE__, __LINE__, __func__, msg, ##__VA_ARGS__)
#define merror_exit(msg, ...) _merror_exit(__FILE__, __LINE__, __func__, msg, ##__VA_ARGS__)


void _mdebug(const char * file, int line, const char * func, const char *msg, ...) __attribute__((format(_PRINTF_FORMAT, 4, 5))) __attribute__((nonnull));
void _minfo(const char * file, int line, const char * func, const char *msg, ...) __attribute__((format(_PRINTF_FORMAT, 4, 5))) __attribute__((nonnull));
void _mwarn(const char * file, int line, const char * func, const char *msg, ...) __attribute__((format(_PRINTF_FORMAT, 4, 5))) __attribute__((nonnull));
void _merror(const char * file, int line, const char * func, const char *msg, ...) __attribute__((format(_PRINTF_FORMAT, 4, 5))) __attribute__((nonnull));
void _merror_exit(const char * file, int line, const char * func, const char *msg, ...) __attribute__((format(_PRINTF_FORMAT, 4, 5))) __attribute__((nonnull)) __attribute__ ((noreturn));
void print_out(const char *msg, ...) __attribute__((format(_PRINTF_FORMAT, 1, 2))) __attribute__((nonnull));


void logging_init(void);

void use_debug();


#endif /* DEBUG_H */
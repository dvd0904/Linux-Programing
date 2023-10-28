#include "shared.h"
#include "cJSON.h"


static int dbg_flag = 0;
// static int chroot_flag = 0;
static int daemon_flag = 0;
static int pid;

static struct {
  unsigned int log_plain:1;
  unsigned int log_json:1;
  unsigned int initialized:1;
} flags;

static pthread_mutex_t logging_mutex;

static void _log(int level, const char * file, int line, const char * func, 
                    const char *msg, va_list args) __attribute__((format(printf, 5, 0))) __attribute__((nonnull));


static void _log(int level, const char * file, int line, 
                    const char * func, const char *msg, va_list args)
{
    FILE *fp;
    char *output;
    char logfile[PATH_MAX + 1];
    char *filename;
    char *timestamp = NULL;
    timestamp = get_timestamp(time(NULL));

    const char *strlevel[5]= {
        "DEBUG",
        "INFO",
        "WARNING",
        "ERROR",
        "CRITICAL"
    };

    if(!flags.initialized)
        logging_init();
    
    if (filename = strrchr(file, '/'), filename) 
        file = filename + 1;
    
    flags.log_plain = 1;
    if(flags.log_plain)
    {
        int oldmask;
        strncpy(logfile, LOGFILE, sizeof(logfile) - 1);
        logfile[sizeof(logfile) - 1] = '\0';

        if (!IsFile(logfile)) 
            fp = fopen(logfile, "a");
        else
        {
            
            oldmask = umask(0006); 
            fp = fopen(logfile, "w");
            umask(oldmask);
        }
        
        if(fp)
        {
            mutex_lock(&logging_mutex);
            (void)fprintf(fp, "%s ", timestamp);

            if (dbg_flag > 0) 
            {
                (void)fprintf(fp, "[%d] %s:%d at %s(): ", pid, file, line, func);
            }

            (void)fprintf(fp, "[%s]: ", strlevel[level]);
            (void)vfprintf(fp, msg, args);
            (void)fprintf(fp, "\n");
            fflush(fp);

            mutex_unlock(&logging_mutex);

            fclose(fp);
        }
    }
    os_free(timestamp);
}


void _minfo(const char * file, int line, const char * func, const char *msg, ...)
{
    va_list args;
    int level = LOGLEVEL_INFO;

    va_start(args, msg);
    _log(level, file, line, func, msg, args);
    va_end(args);
}

void _mwarn(const char * file, int line, const char * func, const char *msg, ...)
{
    va_list args;
    int level = LOGLEVEL_WARNING;
    va_start(args, msg);
    _log(level, file, line, func, msg, args);
    va_end(args);
}

void _mdebug(const char * file, int line, const char * func, const char *msg, ...)
{
    if (dbg_flag >= 1) {
        va_list args;
        int level = LOGLEVEL_DEBUG;
        va_start(args, msg);
        _log(level, file, line, func, msg, args);
        va_end(args);
    }
}

void _merror(const char * file, int line, const char * func, const char *msg, ...)
{
    va_list args;
    int level = LOGLEVEL_ERROR;

    va_start(args, msg);
    _log(level, file, line, func, msg, args);
    va_end(args);
}

void _merror_exit(const char * file, int line, const char * func, const char *msg, ...)
{
    va_list args;
    int level = LOGLEVEL_CRITICAL;

    va_start(args, msg);
    _log(level, file, line, func, msg, args);
    va_end(args);

    exit(1);
}

void print_out(const char *msg, ...)
{
    va_list args;
    va_start(args, msg);

    /* Print to stderr */
    (void)vfprintf(stderr, msg, args);
    (void)fprintf(stderr, "\n");
    
    va_end(args);
}



void logging_init(void)
{
    mutex_init(&logging_mutex, NULL);
    pid = (int)getpid();
}

void nowDebug()
{
    dbg_flag++;
}

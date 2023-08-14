#include "shared.h"


static int dbg_flag = 1;
// static int chroot_flag = 0;
static int daemon_flag = 0;
static int pid;

static struct {
  unsigned int log_plain:1;
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
    char *timestamp = get_timestamp(time(NULL));

    const char *strlevel[4]= {
      "DEBUG",
      "INFO",
      "WARNING",
      "ERROR",
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
        printf("logfile: %s\n", logfile);

        if (!IsFile(logfile)) 
            fp = fopen(logfile, "a");
        else
        {
            printf("vao day\n");
            /* REF: https://www.geeksforgeeks.org/umask-command-in-linux-with-examples/ */
            oldmask = umask(0006); /* set read, write permission for user and groups*/
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

            (void)fprintf(fp, "%s: ", strlevel[level]);
            (void)vfprintf(fp, msg, args);
            (void)fprintf(fp, "\n");
            fflush(fp);

            mutex_unlock(&logging_mutex);

            fclose(fp);
        }

    }
}


void _minfo(const char * file, int line, const char * func, const char *msg, ...)
{
    va_list args;
    int level = LOGLEVEL_INFO;

    va_start(args, msg);
    _log(level, file, line, func, msg, args);
    va_end(args);
}


void logging_init(void)
{
    mutex_init(&logging_mutex, NULL);
    pid = (int)getpid();
}

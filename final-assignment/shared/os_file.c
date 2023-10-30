#include "shared.h"

int IsFile(const char *file)
{
    struct stat buf;
	return (!stat(file, &buf) && S_ISREG(buf.st_mode)) ? 0 : -1;
}

static void _log(char *buf, int pfds, va_list args)
{
    size_t bytes = -1;
    char msg[128];
    vsprintf(msg, buf, args);
    if(bytes = write(pfds, msg, strlen(msg) + 1), bytes <= 0)
        merror(WRITE_ERROR, errno, strerror(errno));
}

void write_to_pipe(pthread_mutex_t * pipe_mutex, int pfds, char *send_buf, ...)
{
    va_list args;
    va_start(args, send_buf);
    mutex_lock(pipe_mutex);
    _log(send_buf, pfds, args);
    mutex_unlock(pipe_mutex);    
    va_end(args);    
}


long get_fp_size(FILE * fp)
{
    long offset;
    long size;

    /* Get initial position */
    if (offset = ftell(fp), offset < 0) {
        return -1;
    }

    /* Move to end */
    if (fseek(fp, 0, SEEK_END) < 0) {
        return -1;
    }

    /* Get ending position */
    if (size = ftell(fp), size < 0) {
        return -1;
    }

    /* Restore original offset */
    if (fseek(fp, offset, SEEK_SET) < 0) {
        return -1;
    }

    return size;
}

char *get_file_content(const char *path, int max_size)
{
    FILE *fp;
    char *content;
    long size;
    size_t read;

    if(path == NULL)
    {
        mdebug("Can not open NULL path");
        goto end;
    }

    /* Load file */
    if (fp = fopen(path, "r"), !fp) {
        mdebug(OPEN_ERROR, errno, strerror(errno));
        goto end;
    }

    /* Get file size */
    if (size = get_fp_size(fp), size < 0) {
        mdebug(SEEK_ERROR, errno, strerror(errno));
        goto end;
    }

    /* Check file size limit */
    if (size > max_size) {
        mdebug("Cannot load file '%s': it exceeds %i MiB", path, (max_size / (1024 * 1024)));
        goto end;
    }

    /* Allocate memory */
    os_malloc(size + 1, content);

    /* Get file content */
    if (read = fread(content, 1, size, fp), read != (size_t)size && !feof(fp)) {
        mdebug(READ_ERROR, errno, strerror(errno));
        os_free(content);
        goto end;
    }

    content[size] = '\0';

end:
    if(fp)
        fclose(fp);

    return content;

}


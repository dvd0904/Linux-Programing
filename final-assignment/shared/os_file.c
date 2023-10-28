#include "shared.h"

int IsFile(const char *file)
{
    struct stat buf;
	return (!stat(file, &buf) && S_ISREG(buf.st_mode)) ? 0 : -1;
}

void write_to_pipe(pthread_mutex_t * pipe_mutex, int pfds, char *send_buf)
{
    mutex_lock(pipe_mutex);
    write(pfds, send_buf, strlen(send_buf) + 1);
    mutex_unlock(pipe_mutex);        
    // free(send_buf);
}


long get_fp_size(FILE * fp)
{
    long offset;
    long size;

    // Get initial position

    if (offset = ftell(fp), offset < 0) {
        return -1;
    }

    // Move to end

    if (fseek(fp, 0, SEEK_END) < 0) {
        return -1;
    }

    // Get ending position

    if (size = ftell(fp), size < 0) {
        return -1;
    }

    // Restore original offset

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


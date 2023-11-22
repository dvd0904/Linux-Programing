#ifndef     OS_FILE_H
#define     OS_FILE_H

#include "shared.h"

/**
 * @brief Check if a file is a regular file
 * @param file file name
 * @return 0 if file is a regular file otherwise -1
*/
int is_file(const char *file);

/**
 * @brief Write buffer to a pipe using variadic function
 * @param pipe_mutex mutex to synchronizartion between multithread
 * @param pfds pipe file descriptor
 * @param send_buf format string
*/
void write_to_pipe(pthread_mutex_t *pipe_mutex, int pfds, char *send_buf, ...);

/**
 * @brief Get size of file
 * @param fp file stream
 * @return size of file
*/
long get_fp_size(FILE * fp);

/**
 * @brief Read content of file
 * @param path file name
 * @param max_size maximum size of file
 * @return a string represent for content of file
*/
char *get_file_content(const char *path, int max_size);

#endif
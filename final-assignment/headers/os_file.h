#ifndef     OSFILE_H
#define     OSFILE_H

#include "shared.h"

int IsFile(const char *file);

void write_to_pipe(pthread_mutex_t * pipe_mutex, int pfds, char *send_buf);

long get_fp_size(FILE * fp);

char *get_file_content(const char *path, int max_size);

#endif
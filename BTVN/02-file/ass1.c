#include <stdio.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include <time.h>
#include <sys/sysmacros.h>

typedef struct stat START;

void getFileType(START st)
{
    if (S_ISDIR(st.st_mode))
    {
        printf("Directory\n");
    }
    else if (S_ISREG(st.st_mode))
    {
        printf("Regular file\n");
    }
    else if (S_ISCHR(st.st_mode))
    {
        printf("Character device\n");
    }
    else if (S_ISBLK(st.st_mode))
    {
        printf("Block device\n");
    }
    else if (S_ISFIFO(st.st_mode))
    {
        printf("FIFO\n");
    }
    else if (S_ISLNK(st.st_mode))
    {
        printf("Symlink\n");
    }
    else if (S_ISSOCK(st.st_mode))
    {
        printf("Socket\n");
    }
}

int main(int argc, char *argv[])
{

    const char *filename = "filesystem.txt";
    char *str = "Konichiwa\n";
    int fd, ret;
    if (fd = open(filename, O_WRONLY | O_CREAT, 0644), fd == -1)
    {
        printf("Unable to open file\n");
        return 1;
    }
    if (ret = write(fd, str, strlen(str)), ret == -1)
    {
        printf("Unable to write to file\n");
        return 1;
    }
    else
        printf("Write to file successfully\n");

    START st;
    if (fstat(fd, &st) == -1)
    {
        printf("Unable to get file information\n");
        return 1;
    }
    printf("File name: %s\n", filename);
    getFileType(st);
    printf("Last modified: %s\n", ctime(&st.st_mtime));
    printf("File size: %ld\n", st.st_size);

    close(fd);
    return 0;
}
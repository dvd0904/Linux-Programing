#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>

int main()
{
    char *filename = "filesystem.txt";
    int fd, ret;
    if (fd = open(filename, O_APPEND | O_CREAT | O_WRONLY, 0644), fd == -1)
    {
        printf("Unable to open file\n");
        return 1;
    }

    if (ret = lseek(fd, 0, SEEK_SET), ret == -1)
    {
        printf("Unable to reposition file\n");
        return 1;
    }
    char *str = "All write operations will always append the data to the end of the file, regardless of the file offset set by lseek\n";
    if (ret = write(fd, str, strlen(str)), ret == -1)
    {
        printf("Unable to write to file\n");
        return 1;
    }
    else
        printf("Write to file successfully\n");
    close(fd);
    return 0;
}
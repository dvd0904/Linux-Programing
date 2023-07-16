#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>

int main()
{
    // Open the file in write mode
    int fileDescriptor = open("dai.txt", O_WRONLY);

    if (fileDescriptor == -1)
    {
        perror("Failed to open the file");
        return 1;
    }

    // Set the file offset to the beginning of the file
    off_t offset = lseek(fileDescriptor, 0, SEEK_SET);

    if (offset == -1)
    {
        perror("Failed to seek to the beginning of the file");
        close(fileDescriptor);
        return 1;
    }

    // Data to write to the file
    const char *data = "dai.";

    // Write the data to the file
    ssize_t bytesWritten = write(fileDescriptor, data, strlen(data));

    if (bytesWritten == -1)
    {
        perror("Failed to write to the file");
        close(fileDescriptor);
        return 1;
    }

    // Close the file
    close(fileDescriptor);

    printf("Data written to the beginning of the file successfully.\n");

    return 0;
}

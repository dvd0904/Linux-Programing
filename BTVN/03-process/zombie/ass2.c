#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <signal.h>

/* Ass2: Sử dụng SIGCHLD để ngăn zombie process */
/* Ass3: Khi 1 child process kết thúc, nó sẽ gửi tới tiến trình cha 1 signal
         gọi là SIGCHLD, tín hiệu này có giá trị = 17
*/

void callback(int signum)
{
    int status;
    printf("I'm callback function\n");
    if (wait(&status) == -1)
    {
        printf("Wait unsuccessfully\n");
    }
    printf("PID: %d ----- PPID: %d ----- Status: %d ----- Signum: %d\n", getpid(), getppid(), status, signum);
}

int main()
{
    pid_t child_pid;

    switch (child_pid = fork(), child_pid)
    {
    case -1:
        printf("Fork unsuccessfully!\n");
        break;
    case 0:
        printf("I'm child process, my PID is: %d and my parent PID is: %d\n", getpid(), getppid());
        printf("Child process is terminated\n");
        while (1)
            ;
        break;
    default:
        signal(SIGCHLD, callback);
        printf("I'm parent process\n");
        while (1)
            ;
        break;
    }
}
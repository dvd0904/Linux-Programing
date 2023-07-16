#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

int main()
{
    pid_t child_pid, ret_pid;
    int status, ret;
    switch (child_pid = fork(), child_pid)
    {
    case -1:
        printf("Error\n");
        break;
    case 0:
        printf("Fork return value: %d\n", child_pid);
        printf("Parent PID: %d\n", getppid());
        printf("I'm child, my PID is: %d\n", getpid());
        while (1)
            ;
        break;

    default:
        // sleep(3);
        if (ret_pid = waitpid(child_pid, &status, NULL), ret_pid == -1)
        {
            printf("Wait unsuccessfully.\n");
        }
        if (WIFEXITED(status))
            printf("Normally termination, status = %d\n", WEXITSTATUS(status));
        else if (WIFSIGNALED(status))
        {
            printf("Child process is killed by signal, status = %d\n", WTERMSIG(status));
        }
        printf("I'm parent, my PID: %d\n", getpid());
        printf("Fork return value, Child PID: %d\n", child_pid);
        break;
    }
}

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

#define NUM_CHILDREN 3

int main(void)
{
    pid_t child_id_arr[NUM_CHILDREN]; 
    int wstatus;
    pid_t cpid, w;

    for (size_t i = 0; i < NUM_CHILDREN; i++)
    {
        if ((child_id_arr[i] = fork()) == -1)
        {
            perror("Can't fork\n");
            exit(1);
        }
        else if (child_id_arr[i] == 0)
        {
            printf("Child: pid = %d, ppid = %d, gid = %d\n", \
                getpid(), getppid(), getpgrp());
            sleep(1);
            exit(EXIT_SUCCESS);
        }
        else
        {
            printf("Parent: pid=%d, childpid=%d, gr=%d\n", getpid(), child_id_arr[i], getpgrp());
        } 
    }

    for (size_t i = 0; i < NUM_CHILDREN; i++)
    {
        do {
            w = waitpid(cpid, &wstatus, WUNTRACED | WCONTINUED);
            if (w == -1) {
                perror("waitpid");
                exit(EXIT_FAILURE);
            }

            if (WIFEXITED(wstatus)) {
                printf("exited, status=%d\n", WEXITSTATUS(wstatus));
            } else if (WIFSIGNALED(wstatus)) {
                printf("killed by signal %d\n", WTERMSIG(wstatus));
            } else if (WIFSTOPPED(wstatus)) {
                printf("stopped by signal %d\n", WSTOPSIG(wstatus));
            } else if (WIFCONTINUED(wstatus)) {
                printf("continued\n");
            }
            } while (!WIFEXITED(wstatus) && !WIFSIGNALED(wstatus));
    }

    return 0;
}

#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>
#include <stdlib.h>

#define NUM_CHILDREN 2

int main(void) 
{
    int status = 0;
    char programs[2][32] = {"main1.out", "main2.out"};
    pid_t child_id_arr[NUM_CHILDREN]; 

    for (int i = 0; i < NUM_CHILDREN; i++)
    {
        if ((child_id_arr[i] = fork()) == -1) 
        {
            perror("Can't fork.");
            exit(1);
        } 
        else if (child_id_arr[i] == 0) 
        {
            if (execl(programs[i], "") == -1)
            {
                perror("Can't exec.\n");
                exit(1);
            }
            // printf("Child process %d. PID = %d, ParentPID = %d, GROUP_ID = %d.\n", i, getpid(), getppid(), getpgrp());
	        return 0;
        }
        else 
        {
            // printf("Parent process. PID = %d, GROUP_ID = %d, child PID = %d.\n", getpid(), getpgrp(), child_id_arr[i]);
        }
    }
    for (int i = 0; i < NUM_CHILDREN; i++)
    {
        if ((child_id_arr[i] = waitpid(child_id_arr[i], &status, 0)) == -1)
        {
            perror("Can't waitpid.\n");
            exit(1);
        }
        // printf("Child process with PID = %d has finished. Status = %d.\n", child_id_arr[i], status);
        if (WIFEXITED(status))
            printf("PID = %d has finished with code = %d.\n", child_id_arr[i], WEXITSTATUS(status));
        else if (WIFSIGNALED(status))
            printf("PID = %d has finished from signal with code = %d.\n", child_id_arr[i], WTERMSIG(status));
        else if (WIFSTOPPED(status))
            printf("PID = %d has stopped with code = %d.\n", child_id_arr[i], WSTOPSIG(status));
    }
    return 0;
}
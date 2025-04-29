#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>
#include <stdlib.h>

#define NUM_CHILDREN 2

int main(void) 
{
    int status = 0;
    char messages[2][128] = {"xxxxxx", "yyyyyyyyyyyyyyyyyyyyyyyyyyyy"};
    int fd[2];
    pid_t child_id_arr[NUM_CHILDREN]; 

    if (pipe(fd) == -1)
    {
        perror("Can't pipe.\n");
        exit(1);
    }

    for (int i = 0; i < NUM_CHILDREN; i++)
    {
        if ((child_id_arr[i] = fork()) == -1) 
        {
            perror("Can't fork.");
            exit(EXIT_FAILURE);
        } 
        else if (child_id_arr[i] == 0) 
        {
            close(fd[0]);
            if (write(fd[1], messages[i], strlen(messages[i])) == -1)
            {
                printf("Error write\n");
                exit(EXIT_SUCCESS);
            }

            printf("Sent message: %s\n", messages[i]);
            // printf("Child process %d. PID = %d, ParentPID = %d, GROUP_ID = %d.\n", i, getpid(), getppid(), getpgrp());
            exit(EXIT_SUCCESS);
        }
    }

    for (int i = 0; i < NUM_CHILDREN; i++)
    {
     	if ((child_id_arr[i] = waitpid(child_id_arr[i], &status, 0)) == -1)
        {
            perror("Can't waitpid.\n");
            exit(1);
        }

    	// printf("Child process with PID = %d has finished. Status = %d.\n", child_id_arr[i], status);\

        if (WIFEXITED(status))
            printf("PID = %d has finished with code = %d.\n", child_id_arr[i], WEXITSTATUS(status));
        else if (WIFSIGNALED(status))
            printf("PID = %d has finished from signal with code = %d.\n", child_id_arr[i], WTERMSIG(status));
        else if (WIFSTOPPED(status))
            printf("PID = %d has stopped with code = %d.\n", child_id_arr[i], WSTOPSIG(status));
    }

    close(fd[1]);
    // char buf[128];

    for (size_t i = 0; i < NUM_CHILDREN + 1; i++)
    {
        char buf[128] = {0};
        read(fd[0], buf, strlen(messages[i]));
        printf("Pipe out: %s\n", buf);
    }

    // while (read(fd[0], buf, sizeof(buf) - 1) > 0)
    //     printf("String from pipe:\n%s\n", buf);

    printf("\n");

    exit(EXIT_SUCCESS);
}
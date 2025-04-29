#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <signal.h>

#define NUM_CHILDREN 2

int flag = 0;

void signal_handler(int sig_numb)
{
    flag = 1;
    printf("\nGot signal %d.\n", sig_numb);
}

int main(void) 
{
    int status = 0;
    char messages[2][128] = {"xxxxxx", "yyyyyyyyyyyyyyyyyyyyyyyyyyyyyy"};
    int fd[2];
    pid_t child_id_arr[NUM_CHILDREN];

    if (pipe(fd) == -1)
    {
        perror("Can't pipe.\n");
        exit(1);
    }

    if (signal(SIGINT, signal_handler) == SIG_ERR)
    {
        perror("Can't signal.\n");
        exit(EXIT_FAILURE);
    }

    sleep(2);

    for (int i = 0; i < NUM_CHILDREN; i++)
    {
        if ((child_id_arr[i] = fork()) == -1) 
        {
            perror("Can't fork.");
            exit(EXIT_FAILURE);
        } 
        else if (child_id_arr[i] == 0) 
        {
            if (flag)
            {
                printf("%d - sent message: %s\n", getpid(), messages[i]);
            	close(fd[0]);
            	if (write(fd[1], messages[i], strlen(messages[i])) == -1)
                {
                    printf("Error write\n");
                    exit(EXIT_FAILURE);
                }
            }
            else
                printf("%d - no signal.\n", getpid());

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

        if (WIFEXITED(status))
            printf("%d finished with code = %d.\n", child_id_arr[i], WEXITSTATUS(status));
        else if (WIFSIGNALED(status))
            printf("%d finished from signal with code = %d.\n", child_id_arr[i], WTERMSIG(status));
        else if (WIFSTOPPED(status))
            printf("%d stopped with code = %d.\n", child_id_arr[i], WSTOPSIG(status));
    }

    close(fd[1]);

    for (size_t i = 0; i < NUM_CHILDREN + 1; i++)
    {
        char buf[128] = {0};
        read(fd[0], buf, strlen(messages[i]));

        printf("String from pipe:\n%s\n", buf);	
    }

    printf("\n");

    exit(EXIT_SUCCESS);
}
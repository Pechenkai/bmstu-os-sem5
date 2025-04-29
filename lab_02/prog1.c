#include <stdio.h>
#include <unistd.h> 
#include <stdlib.h> 
#include <sys/wait.h> 

#define NUM_CHILDREN 3

int main() {
    pid_t child_id_arr[NUM_CHILDREN]; 

    for (int i = 0; i < NUM_CHILDREN; i++) { 
        if ((child_id_arr[i] = fork()) == -1) {

            perror("can`t fork");
            exit(EXIT_FAILURE);  
        } else if (child_id_arr[i] == 0) {
            printf("Child: pid=%d, ppid=%d, gr=%d\n", getpid(), getppid(), getpgrp());
            sleep(2);
            printf("Child: pid=%d, ppid=%d, gr=%d\n", getpid(), getppid(), getpgrp());
            exit(EXIT_SUCCESS);  
        }
        else
            printf("Parent: pid=%d, childpid=%d, gr=%d\n", getpid(), child_id_arr[i], getpgrp());
    }

    return 0;
}
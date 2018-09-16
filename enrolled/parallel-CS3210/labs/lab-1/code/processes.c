/*******************************************************************
 * processes.c
 * Demonstrates process creation in linux.
 * Compile: gcc -o processes processes.c
 * Run: ./processes
 ******************************************************************/

#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>

int value = 10;

int main(int argc, char *argv[])
{
    pid_t fork_ret;

    fork_ret = fork(); //creating the child process

    if (fork_ret > 0) // the parent
        printf("We just cloned a process..!\n");

    if (fork_ret < 0) { //fork_ret would be -1 if unsuccessful
        printf(stderr, "Fork Failed\n");

    }
    else {
        if (fork_ret == 0) { //fork_ret would return 0 in child
            value += 10;
            printf("Child process: value = %d\n",value); 
        }
        else { 
            wait(NULL); //parent waits until child is completed
            printf("Child Completed ....\n");	
            printf("Parent process: value = %d\n", value);
        }
    }

    return 0;
}

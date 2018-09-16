/*************************************
* Lab 2 Exercise 1
* Name:       Jerrell Ezralemuel
* Student No: A0181002B
* Lab Group:  08
*************************************
Warning: Make sure your code works on
lab machine (Linux on x86)
*************************************/

#include <stdio.h>
#include <sys/types.h>   
#include <sys/stat.h>
#include <unistd.h>
#include <sys/wait.h>

int main() {
    int nChild;
    scanf("%d", &nChild);
    pid_t pid[nChild];
    for (int i = 0; i < nChild; i++) {
        pid[i] = fork();
        if (pid[i]) break;
    }
    
    //Wait on child processes in order
    for (int i = 0; i < nChild; i++) {
    }
    return 0;
}

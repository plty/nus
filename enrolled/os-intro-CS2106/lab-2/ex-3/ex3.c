/*************************************
* Lab 2 Exercise 3
* Name:         Jerrell Ezralemuel
* Student No:   A0181002B
* Lab Group:    08
*************************************
* Warning: Make sure your code works on
* lab machine (Linux on x86)
*************************************/

#define PATH_MAX 4096
#define PROC_MAX 4096
#include <stdio.h>
#include <fcntl.h>      //For stat()
#include <sys/types.h>   
#include <sys/stat.h>
#include <sys/wait.h>   //for waitpid()
#include <unistd.h>     //for fork(), wait()
#include <string.h>     //for string comparison etc
#include <stdlib.h>     //for malloc()

bool file_exist (char *path) {
    struct stat buf;
    return stat(path, &buf) == 0;
}

int main() {
    int proc[PROC_MAX];
    char path[PATH_MAX];
    int nProc = 0;
    int status;
    while (printf("YWIMC > "), scanf("%s", path), strcmp(path, "quit")) {
        gets()
        if (!file_exist(path)) {
            printf("%s not found\n", path);
            continue;
        }
        if (!strcmp(path, "printchild")) {
            printf("Unwaited Child Processes:\n");
            for (int i = 0; i < nProc; i++)
                printf("%d\n", proc[i]);
        }

        if (!strcmp(path, "wait")) {
            int pid; scanf("%d", &pid);
            waitpid(pid, &status, 0);
        }

        int pid = fork();
        if (pid == 0)
            execl(path, path, NULL); 
        int status;
        wait(&status);
        printf("\n");
    }
    printf("Goodbye!\n");
    return 0;
}

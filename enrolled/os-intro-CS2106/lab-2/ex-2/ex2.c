/************************************
 * Lab 2 Exercise 2
 * Name:        Jerrell Ezralemuel
 * Student No:  A0181002B
 * Lab Group:   08
 *************************************
 * Warning: Make sure your code works on
 * lab machine (Linux on x86)
 *************************************/

#define PATH_MAX 4096
#include <fcntl.h>      // for stat()
#include <stdbool.h>    // for bool
#include <stdio.h>
#include <string.h>     // for string comparison, string copy
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>   // for wait()
#include <unistd.h>     // for fork()

bool file_exist (char *path) {
    struct stat buf;
    return stat(path, &buf) == 0;
}

int main() {
    char path[PATH_MAX];
    // more readable and more uncluttered
    while (printf("YWIMC > "), scanf("%s", path), strcmp(path, "quit")) {
        if (!file_exist(path)) {
            printf("%s not found\n", path);
            continue;
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

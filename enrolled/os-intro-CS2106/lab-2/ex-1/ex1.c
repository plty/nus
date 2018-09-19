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
    int pid[nChild], i = 0, cur;
    for (; i < nChild; i++) {
        cur = pid[i] = fork();
        if (cur == 0) {
            sleep(1);
            printf("Child %d[%d]: Hello.\n", i + 1, getpid());
            break;
        }
    }

    if (cur != 0)
        for (int i = 0; i < nChild; i++) {
            waitpid(pid[i], NULL, NULL);
            printf("Parent: Child %d[%d] done.\n", i + 1, pid[i]);
        }
    return 0;
}

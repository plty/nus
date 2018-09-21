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
#include <stdbool.h>
int proc[PROC_MAX];
char path[PATH_MAX];
char args[PATH_MAX];
char dump[PATH_MAX];
int nProc = 0;
int result;

bool file_exist (char *path) {
    struct stat buf;
    return stat(path, &buf) == 0;
}

void print_child_cmd() {
    printf("Unwaited Child Processes:\n");
    for (int i = 0; i < nProc; i++)
        printf("%d\n", proc[i]);
}

void wait_cmd(int pid) {
    int i = 0;
    for (i = 0; i < nProc; i++) {
        if (pid == proc[i])
            proc[i] = proc[--nProc];
    }

    if (i == nProc) {
        printf("%d not a valid child\n", pid);
        return;
    }
    int status;
    waitpid(pid, &status, 0);
    result = status >> 8;
}

void result_cmd() {
    printf("%d\n", result);
}

int main() {
    while (printf("YWIMC > "), scanf("%s", path), strcmp(path, "quit")) {
        if (!strcmp(path, "printchild")) {
            print_child_cmd();
        } else if (!strcmp(path, "wait")) {
            int pid; scanf("%d", &pid);
            wait_cmd(pid);
        } else if (!strcmp(path, "result")) {
            result_cmd();
        } else { 
            if (!file_exist(path)) {
                printf("%s not found\n", path);
            }

            // set the first value to nullchar
            args[0] = '\0';
            // this scanf regex is basically scanf if the set of characters is
            // not \n
            scanf("%[^\n]", args);

            int len = strlen(args);
            // if background bg is true
            bool bg = args[len - 1] == '&' && args[len - 2] == ' ';

            // nullchar the & and ' '
            if (bg) {
                args[len - 2] = '\0';
                args[len - 1] = '\0';
            }

            // counting number of arguments
            int argc = 0;
            for (int i = 1; i < len; i++) {
                if (args[i - 1] == ' ' && args[i] != ' ')
                    argc++;
            }

            // setting last in vector to NULL and the first to path
            char *argv[argc + 2];
            argv[0] = path;
            argv[argc + 1] = NULL;
            int cur = 1;

            // if separated by spaces then make them as a separate args by
            // nullchar-ing
            for (int i = 1; i < len; i++) {
                if (args[i - 1] == ' ' && args[i] != ' ') 
                    argv[cur++] = &args[i];
            }

            // nullify spaces s.t. considered as separate string for tokenizing
            for (int i = 0; i < len; i++) {
                if (args[i] == ' ') args[i] = '\0';
            }

            int pid = fork();
            if (pid != 0) {
                proc[nProc++] = pid;
                if (bg) {
                    printf("Child %d in background\n", pid);
                } else { 
                    wait_cmd(pid);
                }
            } else {
                execv(path, argv); 
            }
        }
        printf("\n");
        // dump the rest of the line if not needed
        scanf("%[^\n] ", dump);
    }
    printf("Goodbye!\n");
    return 0;
}

/*************************************
* Lab 2 Exercise 2
* Name:
* Student No:
* Lab Group:
*************************************
Warning: Make sure your code works on
lab machine (Linux on x86)
*************************************/

#include <stdio.h>
#include <string.h>     //For string comparison, string copy
#include <fcntl.h>      //For stat()
#include <sys/types.h>   
#include <sys/stat.h>
#include <unistd.h>     //for fork()
#include <sys/wait.h>   //for wait()

int main()
{
    char path[20];

    //read user input
    printf("YWIMC > ");
    scanf("%s", path );

    while (strcmp( path, "quit") != 0){ 
        // Check whether file exist
        // In real interpreter, a lot more checks are needed
        // E.g. check for file type, execution right etc


        printf("YWIMC > ");
        scanf("%s", path);
    }
    
    printf("Goodbye!\n");
    return 0;
}

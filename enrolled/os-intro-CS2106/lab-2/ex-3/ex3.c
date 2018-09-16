/*************************************
* Lab 2 Exercise 3
* Name:
* Student No:
* Lab Group:
*************************************
Warning: Make sure your code works on
lab machine (Linux on x86)
*************************************/

#include <stdio.h>
#include <fcntl.h>      //For stat()
#include <sys/types.h>   
#include <sys/stat.h>
#include <sys/wait.h>   //for waitpid()
#include <unistd.h>     //for fork(), wait()
#include <string.h>     //for string comparison etc
#include <stdlib.h>     //for malloc()

int main()
{

    char userInput[120];

    //read user input
    printf("YWIMC > ");

    //hint: you probably want to find a better way to read the inputs
    //hint 2: the alternative can be found in one of the sample programs
  
    scanf("%s", userInput);

    while ( strcmp(userInput, "quit") != 0 ){

        printf("YWIMC > ");
        scanf("%s", userInput);
    }
    
    printf("Goodbye!\n");
    return 0;

}

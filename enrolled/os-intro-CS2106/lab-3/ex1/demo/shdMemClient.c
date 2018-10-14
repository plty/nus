/*************************************
* Lab 3 Exercise 1
* Name:
* Student  No:
* Lab Group:
*************************************/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>

int main()
{
    int shdMemId;    
    char* shdMemRegion;

    //Need to know the Share memory region id
    printf("Enter shared memory region id : ");
    scanf("%i", &shdMemId);

    //Attach a new shared memory region
    shdMemRegion = (char*) shmat(shdMemId, NULL, 0);
    if ( shdMemRegion == (char*)-1){
        printf("Cannot attach shared memory region!\n");
        exit(1);
    }

    int* Array = (int*) shdMemRegion;

    //Wait for server to finish writing
    while(Array[0] != 9999){
        sleep(1);           //Wait for a while
    }

    printf("Client: read A[1] = %i, A[2] = %i\n", 
            Array[1], Array[2]);

    Array[1] = 4321;
    Array[2] = 8765;
    Array[3] = 1111;    //Indicate writing is done for the server

    printf("Client: Mischief achived!\n");

    /*Important: Remember to detach the shared memory region*/
    shmdt( shdMemRegion );

    return 0;   //Child Ends here

}

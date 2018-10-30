/*************************************
* Lab 3 Exercise 1
* Name:
* Student No:
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
    int shdMemSize = 4096;  //Size in # of bytes

    //Create a new shared memory region
    shdMemId = shmget( IPC_PRIVATE, shdMemSize, IPC_CREAT | 0666 );
    if (shdMemId < 0){
        printf("Cannot create shared memory region!\n");
        exit(1);
    }
    printf("Shared Memory Id is %i\n",shdMemId);

    //Attach a new shared memory region
    shdMemRegion = (char*) shmat(shdMemId, NULL, 0);
    if ( shdMemRegion == (char*)-1){
        printf("Cannot attach shared memory region!\n");
        exit(1);
    }

    int* Array = (int*) shdMemRegion;

    Array[1] = 1234;
    Array[2] = 5678;
    Array[0] = 9999;    //Indicate writing is done for the client

    printf("Server: I have finished writing!\n");

    //Wait for client to finish writing
    while (Array[3] != 1111){
        sleep(1);
    } 

    printf("Server: read A[1] = %i, A[2] = %i\n", 
            Array[1], Array[2]);

    /*Important: Remember to detach the shared memory region*/
    shmdt( shdMemRegion );

    /*Important: Remember to remove the shared memory region after use!*/
    shmctl(shdMemId, IPC_RMID, NULL); 
    return 0;
}

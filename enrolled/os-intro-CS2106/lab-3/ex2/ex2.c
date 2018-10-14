/*************************************
 * Lab 3 Exercise 2
 * Name:        Jerrell Ezralemuel
 * Student No:  A0181002B
 * Lab Group:   08
 *************************************/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/wait.h>
#include <semaphore.h>
#include <time.h>
#include <math.h>

/** STARTS  ******************************************************
  Semaphore related functions and data structure 
  - Mostly function wrappers for handling an array of semaphores
  in shared memory
 ******************************************************************/
typedef struct {
    sem_t* semArray;
    int shdMemId;
} sharedSemaphore;

//Create "n" semaphores in a shared memory region
void newSemaphoreArray( sharedSemaphore* newSharedSemaphore, int n ) {
    //Create a shared memory region for the semaphores
    newSharedSemaphore->shdMemId = 
        shmget( IPC_PRIVATE, sizeof(sem_t) * n, IPC_CREAT | 0666 );
    if (newSharedSemaphore->shdMemId < 0) {
        perror("Cannot Allocate Shared Memory");
        exit(1);
    }

    //Points the pointers to right place
    newSharedSemaphore->semArray = 
        (sem_t*) shmat(newSharedSemaphore->shdMemId, NULL, 0);
}

//Deallocate the shared memory region used for the semaphores
void destroySempahoreArray( sharedSemaphore* semInfo ) {
    shmdt( (char*)semInfo->semArray );
    shmctl(semInfo->shdMemId, IPC_RMID, NULL); 
}

//Function wrapper for sem_init(). Perform the initialization
// on one of the semaphores
int semaphoreArrayInit( sharedSemaphore semInfo, int which, 
        int pshared, unsigned int value) {
    return sem_init( &(semInfo.semArray[which]), pshared, value);
}

//Function wrapper for sem_wait(). Perform the wait
// on one of the semaphores
void semaphoreArrayWait( sharedSemaphore semInfo, int which ) {
    sem_wait( &semInfo.semArray[which] );
}

//Function wrapper for sem_post(). Perform the wait
// on one of the semaphores
void semaphoreArrayPost( sharedSemaphore semInfo, int which ) {
    sem_post( &semInfo.semArray[which] );
}

/** ENDS *********************************************************
  Semaphore related functions 
 ******************************************************************/

#define N 500000

int main() {

    int shdMemId, result, i, idx;
    int shdMemSize = sizeof(int) * (N+1);  //Size in # of bytes
    int* sharedArray;
    int pCount, cCount, nCount;
    sharedSemaphore mutex;

    //Create a new shared memory region
    shdMemId = shmget( IPC_PRIVATE, shdMemSize, IPC_CREAT | 0666 );
    if (shdMemId < 0) {
        printf("Cannot create shared memory region!\n");
        exit(1);
    }

    //Attach a new shared memory region
    sharedArray = (int*) shmat(shdMemId, NULL, 0);
    if ( sharedArray == (int*)-1) {
        printf("Cannot attach shared memory region!\n");
        exit(1);
    }

    sharedArray[0] = 1; //index for next item

    //Create and initialize binary sempahore

    //Create an "array" of 1 semaphore
    newSemaphoreArray(&mutex, 1 );

    //Initialize the semaphore with a value of "1"
    semaphoreArrayInit(mutex, 0, 1, 1);

    result = fork();
    if (result){        //Parent
        for(i = 0; i < N/2; i ++){

            //only read+write to idx should be protected

            semaphoreArrayWait(mutex, 0);
            idx = sharedArray[0];       
            sharedArray[0] = idx + 1;
            semaphoreArrayPost(mutex, 0);

            sharedArray[idx] = 1111;
        }

    } else {            //Child
        for(i = 0; i < N/2; i ++){

            semaphoreArrayWait(mutex, 0);
            idx = sharedArray[0];
            sharedArray[0] = idx + 1;
            semaphoreArrayPost(mutex, 0);

            sharedArray[idx] = 9999;
        }


        /*Remember to detach the various shared memory regions*/
        destroySempahoreArray(&mutex);

        shmdt( (char*)sharedArray );

        return 0;   //Child Ends here
    }

    //Parent wait for child to finish
    wait(&i);

    /************************************
      Auditing Code - DO NOT CHANGE
     ************************************/
    pCount = cCount = nCount = 0;
    for(i = 1; i < N+1; i++){
        switch(sharedArray[i]){
            case 1111:
                pCount++;
                break;
            case 9999:
                cCount++;
                break;
            default:
                nCount++;
        }
    }
    printf("Audit Result: P = %i, C = %i, N = %i\n", 
            pCount, cCount, nCount);

    //Clean Up 

    // Deallocate the semaphore
    destroySempahoreArray(&mutex);

    /*Important: Remember to detach the shared memory region*/
    shmdt( (char*)sharedArray );

    /*Important: Remember to remove the shared memory region after use!*/
    shmctl(shdMemId, IPC_RMID, NULL); 

    return 0;
}

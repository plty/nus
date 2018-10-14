/*************************************
* Lab 3 Exercise 2
* Name:
* Matric No:
* Lab Group:
*************************************/

#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/wait.h>
#include <semaphore.h>

typedef struct 
{
    sem_t* semPtr;
    int shdMemId;
} semInfoStruct;

void newSemaphore( semInfoStruct* semInfo )
{
    //Create a shared memory region for Semaphore 
    semInfo->shdMemId = shmget( IPC_PRIVATE, sizeof(sem_t), IPC_CREAT | 0666 );
   
    semInfo->semPtr = (sem_t*) shmat(semInfo->shdMemId, NULL, 0);
}

void destroySempahore( semInfoStruct* semInfo )
{
    shmdt( (char*)semInfo->semPtr );
    shmctl(semInfo->shdMemId, IPC_RMID, NULL); 
}

int main()
{
    int result, i;
    semInfoStruct mutex;

    newSemaphore(&mutex);

    sem_init(mutex.semPtr, 1, 1);

    result = fork();
    if (result){        //Parent

        for(i = 0; i < 3; i++){
            printf("p\n");
            sleep(1);
        }

        //wait for child to finish
        wait(&i);

    } else {            //Child

        for(i = 0; i < 3; i++){
            printf("c\n");
            sleep(1);
        }

    }


    destroySempahore(&mutex);

    return 0;
}

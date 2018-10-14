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

int main()
{
    int result, i, sV[3], aV[3];
    int shdMemIdForS;    
    sem_t* semPtr;

    //Create a shared memory region for Semaphore 
    shdMemIdForS = shmget( IPC_PRIVATE, sizeof(sem_t), IPC_CREAT | 0666 );
    semPtr = (sem_t*) shmat(shdMemIdForS, NULL, 0);

    sem_init(semPtr, 1, 1);

    result = fork();
    if (result){        //Parent
        for(i = 0; i < 3; i++){
            //TODO: how to protect?
            printf("p\n");
            sleep(3);
        }

        wait(NULL);
        /*Important: Remember to detach the shared memory region*/
        shmdt( (char*)semPtr );

    } else {            //Child

        for(i = 0; i < 3; i++){
            //TODO: how to protect?

            printf("c\n");
            sleep(1);
        }

       /*Important: Remember to detach the shared memory region*/
        shmdt( (char*)semPtr );
        return 0;   //Child Ends here

    }

    /*Important: Remember to remove the shared memory region after use!*/
    shmctl(shdMemIdForS, IPC_RMID, NULL); 
    return 0;
}

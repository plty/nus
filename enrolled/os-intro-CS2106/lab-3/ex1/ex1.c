/*************************************
 * Lab 3 Exercise 1
 * Name:        Jerrell Ezralemuel
 * Student No:  A0181002B
 * Lab Group:   08
 *************************************/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>  // For Predefined constants
#include <sys/ipc.h>    // For POSIX IPC
#include <sys/shm.h>    // For POSIX Shared Memory

int main()
{
    int result, arraySize, initValue;
    char* shdMemRegion;
    int shdMemId, shdMemSize;

    printf("Enter Array Size: ");
    scanf("%i",&arraySize);

    printf("Enter Start Value: ");
    scanf("%i",&initValue);

    // Create a new shared memory region
    shdMemSize = sizeof(int) * (arraySize + 1);
    shdMemId = shmget(IPC_PRIVATE, shdMemSize, IPC_CREAT | 0666);
    if (shdMemId < 0){
        printf("Cannot create shared memory region!\n");
        exit(1);
    }
    printf("Shared Memory Id is %i\n",shdMemId);

    // Attach a new shared memory region
    shdMemRegion = (char*) shmat(shdMemId, NULL, 0);
    if ( shdMemRegion == (char*) -1){
        printf("Cannot attach shared memory region!\n");
        exit(1);
    }

    int* arr = (int*) shdMemRegion;
    // Initialize the shared memory region 
    for (int i = 0; i < arraySize; i++)
        arr[i] = initValue + i;
    arr[arraySize] = -1;

    // Shared memory regions remained attached after fork()
    // Parent and child can now communicate with each other!
    result = fork();
    if (result){        //Parent

        int parentSum = 0;
        for (int i = arraySize / 2; i < arraySize; i++)
            parentSum += arr[i];

        printf("Parent Sum = %d\n", parentSum);
        while(arr[arraySize] == -1) sleep(0.1);
        int childSum = arr[arraySize];
        printf("Child Sum = %d\n", childSum);
        printf("Total = %d\n", parentSum + childSum);
        shmdt(shdMemRegion);
    } else {

        int childSum = 0; 
        for (int i = 0; i < arraySize / 2; i++)
            childSum += arr[i];
        arr[arraySize] = childSum;
        shmdt(shdMemRegion);
        return 0;

    }

    shmctl(shdMemId, IPC_RMID, NULL); 
    return 0;
}

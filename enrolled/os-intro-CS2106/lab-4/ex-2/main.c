#include <stdio.h>
#include <string.h>
#include "mmalloc.h"

int main()
{
    int* allocated[100]; //at most 100 requests
    int i, heapSize, nRequests, reqType, reqArg, curRequestNo;


    scanf("%i",&heapSize);
	if(!setupHeap( heapSize )){
		return 0;
	}

    scanf("%i",&nRequests);
    for (i = curRequestNo = 0; i < nRequests; i++){
        scanf("%i %i", &reqType, &reqArg);
        switch(reqType){
            case 1:
                //reqArg is the number of integers to be allocated
                //each allocation has a allocation no so that we can
                // deallocate (free) later
                allocated[curRequestNo] = (int*)mymalloc( reqArg * sizeof(int));
                if (allocated[curRequestNo] == NULL){
                    printf("1 %d: failed\n", reqArg);
                }
                curRequestNo++;
                break;
            case 2:
                //reqArg refers to the allocation no
                myfree( allocated[reqArg - 1] );
                break;
        }
#ifdef DEBUG
        //Turning on debug will give you the updated heap partition
        // after every user request

        printHeapMetaInfo();
#endif
    }

    printHeapMetaInfo();

    //Note that we do not print the heap statistic to keep the
    // exercise independent. 

    //printHeapStatistic();

	return 0;
}

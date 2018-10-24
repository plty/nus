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
        scanf("%i", &reqType);
        switch(reqType){
            case 1:
                scanf("%i", &reqArg);
                //reqArg is the number of integers to be allocated
                //each allocation has a allocation no so that we can
                // deallocate (free) later
                allocated[curRequestNo] = (int*)mymalloc( reqArg * sizeof(int));
                if (allocated[curRequestNo] == NULL){
                    printf("1 %d: failed\n", reqArg);
                } else {
                    //We place some values into allocated partition so that we
                    //can verify the compact() later

                    //Place magic number at the first and last location
                    allocated[curRequestNo][0] = curRequestNo + 111;
                    allocated[curRequestNo][reqArg-1] = curRequestNo + 111;
                }
                curRequestNo++;
                break;

            case 2:
                scanf("%i", &reqArg);
                //reqArg refers to the allocation no
                myfree( allocated[reqArg - 1] );
                allocated[reqArg-1] = NULL;
                break;

            case 3:
                compact();
                verifyCompact();
                break;
        }
#ifdef DEBUG
        printHeapMetaInfo();
#endif
    }

    printHeapMetaInfo();

    //Note that we do not print the heap statistic to keep the
    // exercise independent. 

    //printHeapStatistic();

	return 0;
}

#ifndef __MMALLOC_H_	//standard protection against multiple includes
#define __MMALLOC_H_

#include <unistd.h>	//for brk, sbrk

#define FREE 0
#define OCCUPIED 1

typedef struct bm {
	int	size;
	struct bm *nextPart;
	int status;
} partMetaInfo;


typedef struct {
	partMetaInfo* base;
	int totalSize;
	int partMetaSize;
} heapMetaInfo;


int setupHeap(int);
void printHeapMetaInfo();
void printHeapStatistic();

void* mymalloc(int);
void myfree(void*);

void compact();
void verifyCompact();

#endif

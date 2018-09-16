/*******************************************************************
 * race-condition.c
 * Demonstrates race condition.
 * Compile: gcc -pthread -o race race-condition.c
 * Run: ./race
 *******************************************************************/
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#define ADD_THREADS	1000
#define SUB_THREADS     1000

int global_counter;

void *add(void *threadid) {
    long tid;

    tid = (long) threadid;
    global_counter++;
    sleep(rand() % 2);
    printf("add thread #%ld incremented global_counter!\n", tid);
}

void *sub(void *threadid) {
    long tid;

    tid = (long) threadid;
    global_counter--;
    sleep(rand() % 2);
    printf("sub thread #%ld deducted global_counter!\n",tid);
}

int main(int argc, char *argv[]) {
    global_counter = 10;
    pthread_t add_threads[ADD_THREADS];
    pthread_t sub_threads[SUB_THREADS];
    long add_threadid[ADD_THREADS];
    long sub_threadid[SUB_THREADS];

    int rc;
    long t1,t2;
    for(t1 = 0; t1 < ADD_THREADS; t1++) {
        int tid = t1;
        add_threadid[tid] = tid;
        printf("main thread: creating add thread %d\n", tid); rc = pthread_create(&add_threads[tid], NULL, add, (void*)add_threadid[tid]); 
        if (rc) {
            printf("Return code from pthread_create() is %d\n", rc);
            exit(-1);
        }
    }

    for(t2 = 0; t2 < SUB_THREADS; t2++) {
        int tid=t2;
        sub_threadid[tid] = tid;
        printf("main thead: creating sub thread %d\n", tid);
        rc = pthread_create(&sub_threads[tid], NULL, sub, 
                (void *)sub_threadid[tid]);

        if (rc) {
            printf("Return code from pthread_create() is %d\n", rc);
            exit(-1);
        }
    }

    for (int i = 0; i < ADD_THREADS; i++)
        pthread_join(add_threads[i], NULL);
    for (int i = 0; i < SUB_THREADS; i++)
        pthread_join(sub_threads[i], NULL);

    printf("### global_counter final value = %d ###\n", global_counter);
    pthread_exit(NULL);
}

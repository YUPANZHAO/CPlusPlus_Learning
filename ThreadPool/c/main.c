#include <stdio.h>
#include "threadpool.h"
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

void func(void* arg) {
    int* num = (int*) arg;
    printf("thread %ld is working, number = %d\n", pthread_self(), *num);
    sleep(1);
}

int main() {

    ThreadPool* pool = threadPoolCreate(3, 10, 100);

    int i;
    for(i=0; i < 100; ++i) {
        int* num = (int*) malloc(sizeof(int));
        *num = i+100;
        threadPoolAdd(pool, func, num);
    }

    sleep(30);

    threadPoolDestroy(pool);

    return 0;
}
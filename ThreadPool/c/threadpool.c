#include "threadpool.h"
#include <pthread.h>
#include <malloc.h>

//任务结构体
typedef struct Task {
    void (*function) (void* arg);
    void* arg;
}Task;

//线程池结构体
struct ThreadPool {
    // 任务队列
    Task* taskQ;
    int queueCapacity;  //队列容量
    int queueSize;      //当前任务数
    int queueFront;     //队头
    int queueRear;      //队尾

    pthread_t managerID;    //管理者线程ID
    pthread_t* threadIDs;   //工作线程ID
    int minNum;     //最小线程数
    int maxNum;     //最大线程数
    int busyNum;    //忙的线程数
    int liveNum;    //存活线程数
    int exitNum;    //要销毁的线程数

    pthread_mutex_t mutexPool;  //锁整个线程池
    pthread_mutex_t mutexBusy;  //锁busyNum变量
    pthread_cond_t notFull;     //任务队列是不是满了
    pthread_cond_t notEmpty;    //任务队列是不是空了
    
    int shutdown;   //是不是要销毁线程池，销毁为1，不销毁为0
};

ThreadPool* threadPoolCreate(int min, int max, int queueSize) {
    ThreadPool* pool = (ThreadPool*) malloc(sizeof(ThreadPool));
    do {
        if(pool == NULL) {
            printf("malloc threadPool fail...");
            break;
        }
        
        pool->threadIDs = (pthread_t*) malloc(sizeof(pthread_t) * max);
        if(pool->threadIDs == NULL) {
            printf("malloc threadIDs fail...");
            break;
        }
        memset(pool->threadIDs, 0, sizeof(pthread_t) * max);
        pool->minNum = min;
        pool->maxNum = max;
        pool->busyNum = 0;
        pool->liveNum = min;
        pool->exitNum = 0;
        
        if( pthread_mutex_init(&pool->mutexPool, NULL) != 0 ||
            pthread_mutex_init(&pool->mutexPool, NULL) != 0 ||
            pthread_cond_init(&pool->notFull, NULL) != 0 ||
            pthread_cond_init(&pool->notEmpty, NULL) != 0) {
            printf("mutex or cond init fail...");
            break;
        }
        
        //任务队列
        pool->taskQ = (Task*) malloc(sizeof(Task) * queueSize);
        if(pool->taskQ == NULL) {
            printf("malloc taskQ fail...");
            break;
        }
        pool->queueCapacity = queueSize;
        pool->queueSize = 0;
        pool->queueFront = 0;
        pool->queueRear = 0;

        pool->shutdown = 0;

        //创建线程
        pthread_create(&pool->managerID, NULL, manager, NULL);
        int i;
        for(i=0; i < min; ++i) {
            pthread_create(&pool->threadIDs[i], NULL, worker, NULL);
        }
        
        return pool;
    }while(0);
    
    //释放资源
    if(pool && pool->threadIDs) free(pool->threadIDs);
    if(pool && pool->taskQ) free(pool->taskQ);
    if(pool) free(pool);

    return NULL;
}
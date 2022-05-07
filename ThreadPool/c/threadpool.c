#include "threadpool.h"
#include <pthread.h>
#include <malloc.h>

const int NUMBER = 2;

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
        pthread_create(&pool->managerID, NULL, manager, pool);
        for(int i=0; i < min; ++i) {
            pthread_create(&pool->threadIDs[i], NULL, worker, pool);
        }
        
        return pool;
    }while(0);
    
    //释放资源
    if(pool && pool->threadIDs) free(pool->threadIDs);
    if(pool && pool->taskQ) free(pool->taskQ);
    if(pool) free(pool);

    return NULL;
}

void* worker(void* arg) {
    ThreadPool* pool = (ThreadPool*) arg;

    while(1) {
        //加线程池锁
        pthread_mutex_lock(&pool->mutexPool);
        
        //当前队列是否为空
        while(pool->queueSize == 0 && !pool->shutdown) {
            //阻塞工作线程
            pthread_cond_wait(&pool->notEmpty, &pool->mutexPool);
            //判断是不是要销毁线程
            if(pool->exitNum > 0) {
                pool->exitNum--;
                if(pool->liveNum > pool->minNum) {
                    pool->liveNum--;
                    pthread_mutex_unlock(&pool->mutexPool);
                    threadExit(pool);
                }   
            }
        }

        //判断线程池是否被关闭
        if(pool->shutdown) {
            pthread_mutex_unlock(&pool->mutexPool);
            threadExit(pool);
        }

        //从任务队列中取出一个任务
        Task task;
        task.function = pool->taskQ[pool->queueFront].function;
        task.arg = pool->taskQ[pool->queueFront].arg;

        //移动头节点
        pool->queueFront = (pool->queueFront + 1) % pool->queueCapacity;
        pool->queueSize--;

        //解线程池锁
        pthread_cond_signal(&pool->notFull);
        pthread_mutex_unlock(&pool->mutexPool);

        //执行任务
        printf("thread %ld start working...\n", pthread_self());
        pthread_mutex_lock(&pool->mutexBusy);
        pool->busyNum++;
        pthread_mutex_unlock(&pool->mutexBusy);

        task.function(task.arg);
        free(task.arg);
        task.arg = NULL;

        printf("thread %ld end working...\n", pthread_self());
        pthread_mutex_lock(&pool->mutexBusy);
        pool->busyNum--;
        pthread_mutex_unlock(&pool->mutexBusy);
    }
    return NULL;
}

void* manager(void* arg) {
    ThreadPool* pool = (ThreadPool*) arg;
    
    while(!pool->shutdown) {
        //每3s检测一次
        sleep(3);
        
        //取出线程池中任务的数量和当前线程的数量
        pthread_mutex_lock(&pool->mutexPool);
        int queueSize = pool->queueSize;
        int liveNum = pool->liveNum;
        pthread_mutex_unlock(&pool->mutexPool);

        //取出忙的线程数量
        pthread_mutex_lock(&pool->mutexBusy);
        int busyNum = pool->busyNum;
        pthread_mutex_unlock(&pool->mutexBusy);
    
        //添加线程
        //任务的个数>存活的线程个数 && 存活的线程数<最大线程数
        if(queueSize > liveNum && liveNum < pool->maxNum) {
            
            pthread_mutex_lock(&pool->mutexPool);
            int counter = 0;
            for(int i=0; i < pool->maxNum && counter < NUMBER
                && pool->liveNum < pool->maxNum; ++i) {
                if(pool->threadIDs[i] == 0) {
                    pthread_create(&pool->threadIDs[i], NULL, worker, pool);        
                    counter++;
                    pool->liveNum++;
                }
            }
            pthread_mutex_unlock(&pool->mutexPool);
        }

        //销毁线程
        //忙的线程数*2<存活的线程数 && 存活的线程数>最小线程数
        if(busyNum*2 < liveNum && liveNum > pool->minNum) {
            pthread_mutex_lock(&pool->mutexPool);
            pool->exitNum = NUMBER;
            pthread_mutex_unlock(&pool->mutexPool);
            //让工作线程自杀
            for(int i=0; i < NUMBER; ++i) {
                pthread_cond_signal(&pool->notEmpty);
            }
        }
    }

    return NULL;
}

void threadExit(ThreadPool* pool) {
    pthread_t tid = pthread_self();
    for(int i=0; i < pool->maxNum; ++i) {
        if(pool->threadIDs[i] == tid) {
            pool->threadIDs[i] = 0;
            printf("threadExit() called, %ld exiting...\n", tid);
            break;
        }
    }
    pthread_exit(NULL);
}

void threadPoolAdd(ThreadPool* pool, void (*func) (void*), void* arg) {
    pthread_mutex_lock(&pool->mutexPool);
    
    //判断队列是否满了
    while(pool->queueSize == pool->queueCapacity && !pool->shutdown) {
        //阻塞生产者线程
        pthread_cond_wait(&pool->notFull, &pool->mutexPool);
    }

    //判断线程池是否被关闭
    if(pool->shutdown) {
        pthread_mutex_unlock(&pool->mutexPool);
        return;
    }

    //添加任务
    pool->taskQ[pool->queueRear].function = func;
    pool->taskQ[pool->queueRear].arg = arg;

    //移动尾节点
    pool->queueRear = (pool->queueRear + 1) % pool->queueCapacity;
    pool->queueSize++;

    pthread_cond_signal(&pool->notEmpty);
    
    pthread_mutex_unlock(&pool->mutexPool);
}

int threadPoolBusyNum(ThreadPool* pool) {
    pthread_mutex_lock(&pool->mutexBusy);
    int busyNum = pool->busyNum;
    pthread_mutex_unlock(&pool->mutexBusy);
    return busyNum;
}

int threadPoolAliveNum(ThreadPool* pool) {
    pthread_mutex_lock(&pool->mutexPool);
    int aliveNum = pool->liveNum;
    pthread_mutex_unlock(&pool->mutexPool);
    return aliveNum;
}

int threadPoolDestroy(ThreadPool* pool) {
    if(pool == NULL) return -1;
    //关闭线程池
    pool->shutdown = 1;
    //阻塞回收管理者线程
    pthread_join(pool->managerID, NULL);
    //唤醒阻塞的消费者
    for(int i=0; i < pool->liveNum; ++i) {
        pthread_cond_signal(&pool->notEmpty);
    }
    //唤醒阻塞的生产者
    pthread_cond_signal(&pool->notFull);
    //销毁互斥变量和条件变量
    pthread_mutex_destroy(&pool->mutexBusy);
    pthread_mutex_destroy(&pool->mutexPool);
    pthread_cond_destroy(&pool->notEmpty);
    pthread_cond_destroy(&pool->notFull);
    //释放堆内存
    if(pool->taskQ != NULL) free(pool->taskQ);
    if(pool->threadIDs != NULL) free(pool->threadIDs);
    free(pool);
    pool = NULL;
    return 0; 
}
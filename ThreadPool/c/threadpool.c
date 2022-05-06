#include "threadpool.h"
#include <pthread.h>

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


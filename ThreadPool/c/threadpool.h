#ifndef _THREADPOOL_H_
#define _THREADPOOL_H_

typedef struct ThreadPool ThreadPool;

//创建线程池
ThreadPool* threadPoolCreate(int min, int max, int queueSize);

#endif
#ifndef _THREADPOOL_H_
#define _THREADPOOL_H_

typedef struct ThreadPool ThreadPool;

//创建线程池
ThreadPool* threadPoolCreate(int min, int max, int queueSize);

//给线程池添加任务
void threadPoolAdd(ThreadPool* pool, void (*func) (void*), void* arg);

//工作者线程
void* worker(void* arg);

//管理者线程
void* manager(void* arg);

//线程退出函数
void threadExit(ThreadPool* pool);

#endif
#include <iostream>
#include <string.h>
#include <string>
#include "ThreadPool.h"
#include "ThreadPool.cpp"
#include <unistd.h>
#include <pthread.h>

using namespace std;

void func(void* arg) {
    int* num = static_cast<int*>(arg);
    cout << "thread " << to_string(pthread_self()) << " is working, number = " << to_string(*num) << endl;
    sleep(1);
}

int main() {

    ThreadPool<int> pool(3, 10);

    for(int i=0; i < 100; ++i) {
        int* num = new int(i+100);
        pool.addTask(Task<int>(func, num));
    }

    sleep(20);

    return 0;
}
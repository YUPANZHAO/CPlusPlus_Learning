#include <iostream>
#include <cstdlib>
#include <unistd.h>

using namespace std;

/*
    #include <unistd.h>
    
    pid_t fork(void);
    
    函数的作用：
        创建子进程
    返回值:
        父进程里fork()返回子进程的id
        子进程中返回0
        返回-1代表失败
    失败的两个原因：
        1、当前系统的进程数已经达到系统规定的上线，errno的值被设置为EAGAIN
        2、系统内存不足，errno的值被设置为ENOMEM
*/

int main() {
    
    // 创建子进程
    pid_t pid = fork();

    cout << "pid = " << pid << endl;

    // 创建失败
    if(pid == -1) {
        cout << "进程创建失败" << endl;
        exit(1);
    }
    // 当前是子进程
    else if(pid == 0) {
        cout << "我是子进程 当前进程pid = " << getpid() << " 父进程ppid = " << getppid() << endl;
    }
    // 当前是父进程
    else {
        cout << "我是父进程 当前进程pid = " << getpid() << " 父进程ppid = " << getppid() << endl;
    }

    for(int i=0; i < 10; ++i) {
        cout << "i : " << i << " pid : " << getpid() << endl;
        sleep(1);
    }

    return 0;
}
/*
    #include <sys/types.h>
    #include <signal.h>

    int kill(pid_t pid, int sig);
        功能：给任何的进程或进程组pid发送任何信号sig
        参数:
            pid：
                > 0 : 将信号发送给指定进程
                = 0 : 将信号发送给当前的进程组
                = -1 : 将信号发送给每一个有权限接收这个信号的进程
                < -1 : 将信号发送给这个pid的绝对值对应的进程组
            sig：
                需要发送的信号的编号或宏值
        返回值：
            成功（至少发送一个信号）0
            失败 -1

    int raise(int sig);
        功能：给当前进程发送信号
        参数：
            sig：要发送的信号
        返回值：
            成功 0
            失败 -1
    
    void abort(void);
        功能：发送SIGABRT信号给当前进程，杀死当前进程
        相当于kill(getpid(), SIGABRT);
*/

#include <iostream>
#include <sys/types.h>
#include <signal.h>
#include <stdlib.h>
#include <unistd.h>

using namespace std;

int main() {

    // 创建子进程
    pid_t pid = fork();
    
    // 父进程
    if(pid > 0) {
        cout << "i am father" << endl;
        sleep(2);
        cout << "kill child process now" << endl;
        kill(pid, SIGINT);
    }
    // 子进程
    else if(pid == 0) {
        for(int i=0; i < 5; ++i) {
            cout << "i am child!" << endl;
            sleep(1);
        }
    }

    return 0;
}
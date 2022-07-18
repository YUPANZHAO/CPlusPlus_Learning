/*
    SIGCHLD信号产生的3个条件：
        1、子进程结束
        2、子进程暂停
        3、子进程继续运行
    都会给父进程发送该信号，父进程默认忽略该信号

    使用SIGCHLD信号解决僵尸进程的问题
*/

#include <signal.h>
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <wait.h>

using namespace std;

int main() {
    
    // 提前阻塞SIGCHLD信号，因为子进程可能结束了，父进程还没设置好信号捕捉
    sigset_t set;
    sigemptyset(&set);
    sigaddset(&set, SIGCHLD);
    sigprocmask(SIG_BLOCK, &set, NULL);

    // 创建一些子进程
    pid_t pid;
    for(int i=0; i < 20; ++i) {
        pid = fork();
        if(pid == 0) break;
    }

    // 父进程
    if(pid > 0) {
        // 捕捉子进程死亡时发出的SIGCHLD信号
        struct sigaction act;
        act.sa_flags = 0;
        sigemptyset(&act.sa_mask);
        act.sa_restorer = NULL;
        act.sa_handler = [](int sig) {
            cout << "捕捉到了信号 " << sig << endl;
            // 采用循环的方式是因为内核中的 "未决信号集" 只能保存一个信号，其余的会被抛弃
            while(true) {
                // 非阻塞，回收子进程资源
                int pid = waitpid(-1, NULL, WNOHANG);
                if(pid == -1 || pid == 0) break;
                cout << "子进程 " << pid << " 被销毁" << endl;
            }
        };
        sigaction(SIGCHLD, &act, NULL);

        // 注册完信号捕捉后，解除SIGCHLD的信号阻塞
        sigprocmask(SIG_UNBLOCK, &set, NULL);

        // 死循环，目的是让子进程先结束变成僵尸进程
        while(true) {
            cout << "父进程 pid = " << getpid() << endl;
            sleep(2);
        }
    }
    // 子进程
    else if(pid == 0) {
        cout << "子进程 pid = " << getpid() << endl;
    }

    return 0;
}
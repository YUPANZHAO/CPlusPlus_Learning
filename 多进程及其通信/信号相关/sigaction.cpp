/*
    #include <signal.h>

    int sigaction(int signum, const struct sigaction *act,
                    struct sigaction *oldact);
        功能：信号捕捉函数，检查或者改变信号的处理。
        参数：
            signum：需要捕捉的信号
            act：捕捉的信号的处理动作
            oldact：前一次信号的捕捉设置
        返回值：
            成功 0
            失败 -1
    struct sigaction {
        // 回调函数，一个int参数是信号值
        void     (*sa_handler)(int);
        // 另一种回调，不常用
        void     (*sa_sigaction)(int, siginfo_t *, void *);
        // 回调函数处理过程中要阻塞的信号集，处理完后解除阻塞
        sigset_t   sa_mask;
        // 设置0表示选择第一个回调，设置SA_SIGINFO表示选择第二个回调
        int        sa_flags;
        // 已废弃
        void     (*sa_restorer)(void);
    };
*/

#include <signal.h>
#include <stdlib.h>
#include <iostream>
#include <stdio.h>
#include <sys/time.h>

using namespace std;

int main() {

    struct itimerval new_value;
    // 设置间隔时间
    new_value.it_interval.tv_sec = 2;
    new_value.it_interval.tv_usec = 0;
    // 设置延迟时间
    new_value.it_value.tv_sec = 3;
    new_value.it_value.tv_usec = 0;
    // 设置定时器
    int ret = setitimer(ITIMER_REAL, &new_value, NULL); // 非阻塞
    if(ret == -1) {
        perror("setitimer");
        exit(0);
    }

    cout << "定时器3秒后开始运行..." << endl;
    
    // 设置信号处理逻辑
    struct sigaction act;
    act.sa_handler = [](int sig) {
        cout << "ALARM CALL " << sig << endl;
    };
    act.sa_flags = 0;
    sigemptyset(&act.sa_mask);
    act.sa_restorer = NULL;

    // 设置信号捕捉
    ret = sigaction(SIGALRM, &act, NULL);
    if(ret == -1) {
        perror("sigaction");
        exit(0);
    }
    
    while(true) {}

    return 0;
}
/*
    #include <signal.h>

    typedef void (*sighandler_t)(int);

    sighandler_t signal(int signum, sighandler_t handler);

    功能：设置某个信号的捕捉行为
    参数:
        signum：要捕捉的信号
        handler：捕捉到信号的处理动作
            - SIG_IGN : 忽略信号
            - SIG_DFL : 使用信号默认行为
            - 自定义回调函数
    返回值：
        成功：返回前一次设置的handler
        失败：SIG_ERR

    SIGKILL、SIGSTOP不能被捕捉和忽略
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
    
    // 设置信号捕捉
    auto sig_ret = signal(SIGALRM, [](int sig) {
        cout << "ALARM CALL " << sig << endl; 
    });
    if(sig_ret == SIG_ERR) {
        perror("signal");
        exit(0);
    }
    
    getchar();

    return 0;
}
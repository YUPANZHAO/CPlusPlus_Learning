/*
    #include <signal.h>

    int sigemptyset(sigset_t *set);
        功能：清空信号集中的数据，将信号集中的所有标志位都置为0
        参数：传出参数，需要操作的信号集
        返回值：成功 0，失败 -1

    int sigfillset(sigset_t *set);
        功能：将信号集中的所有标志位都置为1
        参数：传出参数，需要操作的信号集
        返回值：成功 0，失败 -1

    int sigaddset(sigset_t *set, int signum);
        功能：设置信号集中对应信号标志位为1，表示阻塞这个信号
        参数：
            set：传出参数，需要操作的信号集
            signum：需要阻塞的信号
        返回值：成功 0，失败 -1

    int sigdelset(sigset_t *set, int signum);
        功能：设置信号集中对应信号标志位为0，表示不阻塞这个信号
        参数：
            set：传出参数，需要操作的信号集
            signum：需要不阻塞的信号
        返回值：成功 0，失败 -1

    int sigismember(const sigset_t *set, int signum);
        功能：判断某个信号是否阻塞
        参数：
            set：信号集
            signum：需要判断的信号
        返回值：
            1：signum被阻塞
            0：signum不阻塞
            -1：调用失败
*/

#include <signal.h>
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

using namespace std;

int main() {

    // 创建信号集
    sigset_t set;

    // 清空信号集
    sigemptyset(&set);
    
    // 判断SIGINT是否在信号集set中
    int ret = sigismember(&set, SIGINT);
    if(ret == 0) {
        cout << "SIGINT 不阻塞" << endl;
    }else if(ret == 1) {
        cout << "SIGINT 阻塞" << endl;
    }

    // 添加几个信号到信号集
    sigaddset(&set, SIGINT);
    sigaddset(&set, SIGQUIT);
    
    // 判断SIGINT是否在信号集set中
    ret = sigismember(&set, SIGINT);
    if(ret == 0) {
        cout << "SIGINT 不阻塞" << endl;
    }else if(ret == 1) {
        cout << "SIGINT 阻塞" << endl;
    }

    // 判断SIGQUIT是否在信号集set中
    ret = sigismember(&set, SIGQUIT);
    if(ret == 0) {
        cout << "SIGQUIT 不阻塞" << endl;
    }else if(ret == 1) {
        cout << "SIGQUIT 阻塞" << endl;
    }

    // 从信号集中删除信号
    sigdelset(&set, SIGQUIT);

    // 判断SIGINT是否在信号集set中
    ret = sigismember(&set, SIGINT);
    if(ret == 0) {
        cout << "SIGINT 不阻塞" << endl;
    }else if(ret == 1) {
        cout << "SIGINT 阻塞" << endl;
    }

    // 判断SIGQUIT是否在信号集set中
    ret = sigismember(&set, SIGQUIT);
    if(ret == 0) {
        cout << "SIGQUIT 不阻塞" << endl;
    }else if(ret == 1) {
        cout << "SIGQUIT 阻塞" << endl;
    }

    return 0;
}
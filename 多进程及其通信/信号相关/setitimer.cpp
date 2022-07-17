/*
    #include <sys/time.h>

    int setitimer(int which, const struct itimerval *new_value,
                    struct itimerval *old_value);

    功能：设置定时器，替代alarm。精度高(us)，实现周期性定时器。
    参数：
        which：定时器以什么时间计时
            ITIMER_REAL : 真实时间，发送SIGALRM
            ITIMER_VIRTUAL : 用户时间，发送SIGVTALRM
            ITIMER_PROF :   以该进程在用户态和内核态下所消耗的时间计算，发送SIGPROF
        
        new_value：定时器的属性
            struct itimerval {              // 定时器结构体
                struct timeval it_interval; // 间隔时间
                struct timeval it_value;    // 延迟多久开启定时器
            };
            struct timeval {                // 时间结构体
                time_t      tv_sec;         // 秒数
                suseconds_t tv_usec;        // 微妙
            };
        
        old_value：传出参数，记录上一次定时器的属性
    返回值：
        成功 0
        失败 -1 并设置errno
*/

#include <sys/time.h>
#include <iostream>
#include <stdlib.h>
#include <stdio.h>

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

    getchar();

    return 0;
}
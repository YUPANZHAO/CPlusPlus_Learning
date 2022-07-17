/*
    #include <unistd.h>

    unsigned int alarm(unsigned int seconds);
        
    功能：设置定时器，设定一段时间后给当前进程发送一个信号SIGALARM
    参数：seconds：倒计时的时长，单位：秒。如果参数为0，则取消一个定时器，通过alarm(0)
    返回值：
        之前没有定时器，返回0
        之前有定时器，返回之前的定时器剩余时间
        
    SIGALARM：默认终止当前的进程，每一个进程都有且仅有一个唯一的定时器
        alarm(10); -> 返回0
        过了1秒
        alarm(5);  -> 返回9
*/

#include <iostream>
#include <unistd.h>

using namespace std;

int main() {

    int seconds = alarm(5);
    
    cout << "seconds = " << seconds << endl;

    sleep(2);

    seconds = alarm(5); // 不阻塞

    cout << "seconds = " << seconds << endl;

    while(true) {
        
    }

    return 0;
}
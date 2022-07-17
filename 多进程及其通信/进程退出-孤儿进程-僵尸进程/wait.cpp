#include <unistd.h>
#include <sys/types.h>
#include <wait.h>
#include <iostream>
#include <stdlib.h>

using namespace std;

/*
    #include <sys/types.h>
    #include <sys/wait.h>

    pid_t wait(int *status);
        参数：
            status：传出参数
        返回值：
            成功：返回被回收的子进程id
            失败：-1（所有子进程都结束或调用失败）
            可以通过一些宏定义判断是否获取状态，如WIFEXITED、WEXITSTATUS等
*/

int main() {

    pid_t pid;
    
    // 创建5个子进程
    for(int i=0; i < 5; ++i) {
        pid = fork();
        if(pid == 0) break;
    }

    // 父进程
    if(pid > 0) {
        while(true) {
            cout << "我是父进程, pid = " << getpid() << endl;

            int status;
            int ret = wait(&status);
            
            // 没有子进程了
            if(ret == -1) {
                break;
            }
            // 一个子进程正常退出
            else if(WIFEXITED(status)) {
                cout << "子进程" << ret << "正常退出, 退出的状态: " << WEXITSTATUS(status) << endl;
            }
            // 一个子进程被信号干掉了
            else if(WIFSIGNALED(status)) {
                cout << "子进程" << ret << "被信号" << WTERMSIG(status) << "干掉了" << endl; 
            }

            sleep(1);
        }
    }
    // 子进程
    else if(pid == 0) {
        sleep(1);
        // 可以通过kill -9干掉
        // while(true) {
        //     sleep(1);
        //     cout << "我是子进程, pid = " << getpid() << endl;
        // }
        cout << "我是子进程, pid = " << getpid() << endl;
        exit(0);
    }

    return 0;
}
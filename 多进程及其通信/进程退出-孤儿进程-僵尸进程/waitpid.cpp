#include <unistd.h>
#include <sys/types.h>
#include <wait.h>
#include <iostream>
#include <stdlib.h>

using namespace std;

/*
    #include <sys/types.h>
    #include <sys/wait.h>

    pid_t waitpid(pid_t pid, int *status, int options);
    参数：
        pid:
            > 0 : 回收指定pid的子进程
            = 0 : 回收当前进程所在进程组的所有子进程
            = -1 : 回收所有子进程
            < -1 : 回收其子进程的进程组为pid的绝对值的所有子进程
        status：
            传出参数，返回的进程信息句柄，可通过宏获取具体信息
        options:
            0 : 阻塞
            WNOHANG : 非阻塞
        返回值：
            > 0 : 返回子进程的id
            = 0 : option = WNOHANG，表示还有子进程活着，但是还在运行
            = -1 : 错误，或者没有子进程了
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
            // 非阻塞
            int ret = waitpid(-1, &status, WNOHANG);
            
            // 没有子进程了
            if(ret == -1) {
                break;
            }
            // 没有子进程退出
            else if(ret == 0) {
                sleep(1);
                continue;    
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
        while(true) {
            sleep(1);
            cout << "我是子进程, pid = " << getpid() << endl;
        }
        // cout << "我是子进程, pid = " << getpid() << endl;
        // exit(0);
    }

    return 0;
}
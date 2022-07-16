#include <unistd.h>
#include <iostream>

using namespace std;

/*
    #include <unistd.h>

    int execvp(const char *file, char *const argv[]);

    参数：
        file：需要指定的可执行文件的名称
            会在环境变量PATH中查找该可执行文件，若未找到则调用失败
        argv：执行可执行文件的参数列表，通过指针数组传递
    返回值：
        只有调用失败才有返回值，返回-1，并且设置errno
        如果调用成功，没有返回值
*/

int main() {
    
    // 创建一个子进程，在子进程中执行exec函数族中的函数
    pid_t pid = fork();

    // 父进程
    if(pid > 0) {
        cout << "我是父进程 当前进程pid = " << getpid() << endl;
    }
    // 子进程
    else if(pid == 0) {
        cout << "我是子进程 当前进程pid = " << getpid() << endl;
        char * argv[] = {"ps", "a", "u", "x", NULL};
        execvp("ps", argv);
        cout << "我想要输出!!!(输出了意味着execvp调用失败)" << endl;
    }

    sleep(1);

    return 0;
}
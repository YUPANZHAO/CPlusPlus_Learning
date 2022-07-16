#include <unistd.h>
#include <iostream>

using namespace std;

/*
    #include <unistd.h>

    int execv(const char *path, char *const argv[]);

    参数：
        path：需要指定的执行文件路径
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
        char * argv[] = {"hello", "Tom", NULL};
        execv("hello", argv);
        cout << "我想要输出!!!(输出了意味着execv调用失败)" << endl;
    }

    sleep(1);

    return 0;
}
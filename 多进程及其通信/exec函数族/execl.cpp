#include <unistd.h>
#include <iostream>

using namespace std;

/*
    #include <unistd.h>

    int execl(const char *path, const char *arg, ...);

    参数：
        path：需要指定的执行文件路径
        arg：执行可执行文件的参数列表
            第一个参数配什么用，为了方便一般写可执行文件的名称
            从第二个参数开始，就是程序的参数列表
            参数最后需要以NULL结束（哨兵）
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
        execl("hello", "hello", "Tom", NULL);
        cout << "我想要输出!!!(输出了意味着execl调用失败)" << endl;
    }

    sleep(1);

    return 0;
}
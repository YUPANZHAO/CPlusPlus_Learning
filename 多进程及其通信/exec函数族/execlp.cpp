#include <unistd.h>
#include <iostream>

using namespace std;

/*
    #include <unistd.h>

    int execlp(const char *file, const char *arg, ...);

    参数：
        file：需要指定的可执行文件的文件名
            会到环境变量中查找指定的可执行文件，如果找到了就执行，找不到就执行不成功
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
        execlp("ps", "ps", "aux", NULL);
        cout << "我想要输出!!!(输出了意味着execlp调用失败)" << endl;
    }

    sleep(1);

    return 0;
}
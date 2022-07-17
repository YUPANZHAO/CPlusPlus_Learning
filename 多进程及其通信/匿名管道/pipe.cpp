#include <unistd.h>
#include <iostream>
#include <string.h>
using namespace std;

/*
    #include <unistd.h>

    int pipe(int pipefd[2]);
        功能：创建一个匿名管道，用来进程间通信
        参数：int pipefd[2]是一个传出参数
            pipefd[0] 对应管道的读端
            pipefd[1] 对应管道的写段
        返回值：
            成功 0
            失败 -1
        PS：匿名管道只能用于具有关系的进程间通信（父子进程、兄弟进程）
*/

// 子进程发送数据给父进程，父进程读取到数据输出
int main() {
    
    // 在fork之前创建管道
    int pipefd[2];
    int ret = pipe(pipefd);
    if(ret == -1) {
        cerr << "create pipe error" << endl;
        return 0;
    }

    // 创建子进程
    pid_t pid = fork();

    // 父进程
    if(pid > 0) {
        close(pipefd[1]);
        // 从管道的读取端读取数据
        char buf[1024] = {0};
        int len = read(pipefd[0], buf, sizeof(buf));
        cout << "父进程读取到的数据: " << buf << ", pid = " << getpid() << endl;
    }
    // 子进程
    else if(pid == 0) {
        close(pipefd[0]);
        // 从管道的写端写入数据
        char *str = "hello, i am child";
        write(pipefd[1], str, strlen(str));
    }

    return 0;
}
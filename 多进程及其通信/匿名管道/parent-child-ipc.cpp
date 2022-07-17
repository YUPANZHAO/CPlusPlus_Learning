#include <unistd.h>
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <wait.h>

using namespace std;

/*
    实现 ps aux | grep xxx 父子进程间通信
    
    子进程：ps aux，子进程结束后，将数据发送给父进程
    父进程：获取到数据，过滤数据
    
    原理：
        通过execlp执行ps aux命令
        通过dup2重定向标准输出到指定管道写段
        通过pipe进行父子进程间的数据传输
*/

int main() {

    // 创建一个管道
    int fd[2];
    int ret = pipe(fd);
    if(ret == -1) {
        cerr << "create pipe error" << endl;
        exit(0);
    }

    // 创建子进程
    pid_t pid = fork();
    
    // 父进程
    if(pid > 0) {
        // 关闭写端
        close(fd[1]);
        // 从管道中读取数据
        char buf[1024] = {0};
        int len = -1;
        while((len = read(fd[0], buf, sizeof(buf)-1)) > 0) {
            // 过滤管道数据
            cout << buf;
            memset(buf, 0, sizeof(buf));
        }
        // 回收子进程资源
        wait(NULL);
    }
    // 子进程
    else if(pid == 0) {
        // 关闭读端
        close(fd[0]);
        // 文件描述符重定向
        dup2(fd[1], STDOUT_FILENO);
        // 执行ps aux
        execlp("ps", "ps", "aux", NULL);
        // 执行失败
        cerr << "execlp error" << endl;
        exit(0);
    }
    // 错误
    else {
        cerr << "fork error" << endl;
        exit(0);
    }

    return 0;
}
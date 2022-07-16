#include <unistd.h>
#include <iostream>

using namespace std;

/*
    #include <unistd.h>

    int execve(const char *filename, char *const argv[], char *const envp[]);

    参数：
        filename：需要指定的可执行文件的名称
            可以传递一个指向环境字符串指针数组的指针，可执行文件通过全局environ变量接收，其余与execl无差别
            注意envp最后一个参数要为NULL
        argv：执行可执行文件的参数列表，通过指针数组传递
        envp：存有环境变量字符串地址的指针数组的地址
            如 char *envp[] = {"/home/zyp/aaa", "/home/zyp/bbb", "/home/zyp/ccc", NULL};
            必须以NULL结束
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
        char * envp[] = {"AA=aa", "BB=bb", "CC=cc", NULL};
        execve("echoenv", argv, envp);
        cout << "我想要输出!!!(输出了意味着execve调用失败)" << endl;
    }

    sleep(1);

    return 0;
}
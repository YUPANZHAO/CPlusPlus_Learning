#include <unistd.h>
#include <iostream>

using namespace std;

/*
    #include <unistd.h>

    extern char **environ;

    int execle(const char *path, const char *arg, ..., char * const envp[]);

    参数：
        path：需要指定的执行文件路径
            可以传递一个指向环境字符串指针数组的指针，可执行文件通过全局environ变量接收，其余与execl无差别
            注意envp最后一个参数要为NULL
        arg：执行可执行文件的参数列表
            第一个参数配什么用，为了方便一般写可执行文件的名称
            从第二个参数开始，就是程序的参数列表
            参数最后需要以NULL结束（哨兵）
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
        char * envp[] = {"AA=aa", "BB=bb", "CC=cc", NULL};
        execle("./echoenv", "echoenv", NULL, envp);
        cout << "我想要输出!!!(输出了意味着execle调用失败)" << endl;
    }

    sleep(1);

    return 0;
}
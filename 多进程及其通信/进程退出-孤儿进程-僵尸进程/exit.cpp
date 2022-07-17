#include <stdlib.h>
#include <iostream>
#include <unistd.h>

using namespace std;

/*
    #include <stdlib.h>
    int exit(int status);
    在退出进程时会向父进程传递status，刷新输出缓冲区，并调用底层的_exit()

    #include <unistd.h>
    int _exit(int status);
    直接执行系统调用，不会刷新输出缓冲区
*/

int main() {

    cout << "hello\n";
    cout << "world!";

    // exit(0);
    _exit(0); // world!并没有输出，因为没有刷新输出缓冲区

    return 0;
}
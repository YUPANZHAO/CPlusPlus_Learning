#include <iostream>
#include <unistd.h>
using namespace std;

int main() {

    pid_t pid = fork();

    // 父进程
    if(pid > 0) {
        cout << "我是父进程, pid = " << getpid() << endl;
        while(true) {
            sleep(1);
        }
    }
    // 子进程 
    else if(pid == 0) {
        cout << "我是子进程, pid = " << getpid() << endl;
    }

    return 0;
}
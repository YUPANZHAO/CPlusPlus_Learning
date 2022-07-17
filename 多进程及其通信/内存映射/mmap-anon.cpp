#include <iostream>
#include <sys/mman.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <string.h>
#include <fcntl.h>
#include <wait.h>

using namespace std;

/*
    匿名映射，通过设置mmap中的flags参数为MAP_ANONYMOUS，使得创建内存映射时忽略fd、offset
*/

int main() {
    
    // 创建匿名内存映射区
    int len = 4096;
    void *ptr = mmap(NULL, len, PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0);
    if(ptr == MAP_FAILED) {
        perror("mmap");
        exit(0);
    }
    
    // 创建子进程
    pid_t pid = fork();
    
    // 父进程
    if(pid > 0) {
        // 等待回收子进程
        wait(NULL);
        
        char buf[64];
        strcpy(buf, (char *)ptr);
        cout << "read data: " << buf << endl;
    }
    // 子进程
    else if(pid == 0) {
        strcpy((char *)ptr, "nihao, son!!!");
    }

    // 关闭内存映射区
    munmap(ptr, len);

    return 0;
}
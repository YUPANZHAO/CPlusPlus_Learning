#include <iostream>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

using namespace std;

/*
    通过内存映射与没有关系的进程之间进行通讯（读数据部分）
*/

int main() {

    // 打开文件
    int fd = open("test.txt", O_RDWR);
    
    // 获取文件大小
    int size = lseek(fd, 0, SEEK_END);

    // 创建内存映射
    void *ptr = mmap(NULL, size, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    if(ptr == MAP_FAILED) {
        perror("mmap");
        exit(0);
    }

    // 读数据
    cout << "recv buf: " << (char *)ptr;

    // 释放内存映射
    munmap(ptr, size);

    // 关闭文件
    close(fd);

    return 0;
}
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>

using namespace std;

/*
    使用内存映射实现文件拷贝功能
    
    思路：
        1、对原始的文件进行内存映射
        2、创建一个新文件（同时扩展文件大小）
        3、把新文件的数据映射到内存中
        4、通过内存拷贝将第一个文件的内存数据拷贝到新的文件内存中
        5、释放资源
*/

int main() {

    // 打开原始文件
    int fd = open("test.txt", O_RDWR);
    if(fd == -1) {
        perror("open");
        exit(0);
    }
    
    // 获取文件大小
    int size = lseek(fd, 0, SEEK_END);

    // 创建一个新的文件
    int fd1 = open("cpy.txt", O_RDWR | O_CREAT, 0664);
    if(fd1 == -1) {
        perror("open");
        exit(0);
    }
    
    // 对新文件进行扩展
    truncate("cpy.txt", size);
    write(fd1, " ", 1);
    
    // 分别做内存映射
    void *ptr = mmap(NULL, size, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    if(ptr == MAP_FAILED) {
        perror("map");
        exit(0);
    }
    void *ptr1 = mmap(NULL, size, PROT_READ | PROT_WRITE, MAP_SHARED, fd1, 0);
    if(ptr1 == MAP_FAILED) {
        perror("map");
        exit(0);
    }

    // 内存拷贝
    memcpy(ptr1, ptr, size);

    // 释放内存映射
    munmap(ptr1, size);
    munmap(ptr, size);

    // 关闭文件
    close(fd1);
    close(fd);

    return 0;
}
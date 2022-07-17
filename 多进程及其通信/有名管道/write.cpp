#include <iostream>
#include <sys/types.h>
#include <sys/stat.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>

using namespace std;

/*
    #include <sys/types.h>
    #include <sys/stat.h>

    int mkfifo(const char *pathname, mode_t mode);
        参数：
            pathname：管道名称的路径
            mode：文件的权限（和open的mode一样）
*/

int main() {

    // 判断文件是否存在
    int ret = access("fifo", F_OK);
    if(ret == -1) {
        // 创建管道文件
        ret = mkfifo("fifo", 0664);
        if(ret == -1) {
            perror("mkfifo");
            exit(0);
        }
    }

    // 打开管道
    int fd = open("fifo", O_WRONLY);
    if(fd == -1) {
        perror("open");
        exit(0);
    }

    // 写数据
    for(int i=0; i < 100; ++i) {
        char buf[1024];
        sprintf(buf, "hello, %d\n", i);
        cout << "write data: " << buf;
        write(fd, buf, strlen(buf));
        sleep(1);
    }

    // 关闭文件
    close(fd);

    return 0;
}
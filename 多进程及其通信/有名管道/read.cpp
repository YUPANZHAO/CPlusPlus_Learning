#include <iostream>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

using namespace std;

int main() {

    // 打开管道文件
    int fd = open("fifo", O_RDONLY);
    if(fd == -1) {
        perror("open");
        exit(0);
    }

    // 读数据
    while(true) {
        char buf[1024] = {0};
        int len = read(fd, buf, sizeof(buf));
        if(len == 0) {
            cout << "写端断开了连接..." << endl;
            exit(0);
        }
        cout << "recv buf: " << buf;
    }

    // 关闭文件
    close(fd);

    return 0;
}
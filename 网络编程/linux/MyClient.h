#ifndef _MyClient_H_
#define _MyClient_H_

#include <iostream>
#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <errno.h>
#include <cstring>

using namespace std;

class MyClient {

private:

    // IP地址
    string host;
    // 端口号
    int port;
    // 服务器套接字句柄
    int server_socket;
    // 服务器地址信息
    sockaddr_in server_addr;
    // 错误信息
    string error_message;
    // 缓冲区大小
    const int BUFFER_SIZE = 40;

    // 更新错误信息
    void flushError();
    
public:

    // 无参构造函数
    MyClient() : port(4331), host("127.0.0.1") {}; 
    // 有参构造函数 仅存储数据，不进行连接
    MyClient(int port, string host = "127.0.0.1")
        : port(port), host(host) {};
    // 析构函数 释放
    ~MyClient();
    // 连接服务端
    bool Connect();
    // 发送数据，返回发送数据字节数，-1表示错误
    int Send(string message);
    // 接收数据, 返回一个string, 返回NULL代表错误
    string Recv();
    // 获取最新发生的错误信息
    string Error();

};

#endif
#include <iostream>
#include <cstring>
#include "MyServer.h"
using namespace std;

MyServer server;

int main() {
    if(server.Listen(32) == false) {
        cout << "服务端异常" << endl;
        cout << "错误消息：" << server.Error() << endl;
        return -1;
    }else {
        cout << "服务端启动监听" << endl;
    }

    while(true) {
        cout << "等待客户端连接..." << endl;
        MyServer::ClientInfo client = server.Accept();
        if(client.socket == -1) {
            cout << "客户端连接异常" << endl;
            cout << "错误消息：" << server.Error() << endl;
            return -1;
        }else {
            cout << "客户端已连接" << endl;
            cout << "客户端信息：" << inet_ntoa(client.addr.sin_addr) << ":" << ntohs(client.addr.sin_port) << endl;
        }        

        string mes = server.Recv(client);
        if(mes.data() == NULL) {
            cout << "消息接收失败" << endl;
            cout << "错误消息：" << server.Error() << endl;
            return -1;
        }else {
            cout << "接收到客户端消息" << endl;
            cout << "消息内容：" << mes << endl;
        }
        
        if(server.Send(client, mes) == -1) {
            cout << "消息回传失败" << endl;
            cout << "错误消息：" << server.Error() << endl;
            return -1;
        }else {
            cout << "消息回传完成" << endl;
        }

        server.ShutDown(client);
    }
    return 0;
}
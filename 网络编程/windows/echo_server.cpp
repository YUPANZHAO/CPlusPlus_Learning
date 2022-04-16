#include <WinSock2.h>
#pragma comment(lib, "ws2_32")
#include <iostream>
using namespace std;

const int PORT = 4331;
const char* HOST = "127.0.0.1";

int main() {

    WSADATA data;
    int ret = WSAStartup(MAKEWORD(2,2), &data);
    if(ret) {
        cout << "初始化网络错误！" << endl;
        WSACleanup();
        return -1;
    }
    cout << "初始化网络成功！" << endl;
    
    SOCKET server_socket = socket(AF_INET, SOCK_STREAM, 0);
    if(server_socket == -1) {
        cout << "创建套接字失败！" << endl;
        WSACleanup();
        return -1;
    }
    cout << "创建套接字成功！" << endl;

    sockaddr_in server_addr;
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    server_addr.sin_addr.S_un.S_addr = inet_addr(HOST);
    
    ret = bind(server_socket, (sockaddr *)&server_addr, sizeof(server_addr));
    if(ret == -1) {
        cout << "绑定地址端口失败！" << endl;
        WSACleanup();
        return -1;
    }
    cout << "绑定地址端口成功！" << endl;

    ret = listen(server_socket, 5);
    if(ret == -1) {
        cout << "监听套接字失败！" << endl;
        WSACleanup();
        return -1;
    }
    cout << "监听套接字成功！" << endl;

    while(true) {

        sockaddr client_addr;
        int len = sizeof(client_addr);
        SOCKET client_socket = accept(server_socket, (sockaddr *)&client_addr, &len);
        if(client_socket == -1) {
            cout << "接收客户端连接失败！" << endl;
            WSACleanup();
            return -1;
        }
        cout << "客户端连接成功！" << endl;

        const int BUFF_SIZE = 100;
        char buf[BUFF_SIZE];
        
        ret = recv(client_socket, buf, BUFF_SIZE, 0);
        if(ret <= 0) {
            cout << "接受客户端数据失败！" << endl;
            WSACleanup();
            return -1;
        }
        buf[ret] = '\0';
        cout << "Client: " << buf << endl;

        ret = send(client_socket, buf, strlen(buf), 0);
        if(ret == -1) {
            cout << "回传信息失败！" << endl;
            WSACleanup();
            return -1;
        }
        cout << "回传信息成功！" << endl;

    }

    WSACleanup();
    
    return 0;
}
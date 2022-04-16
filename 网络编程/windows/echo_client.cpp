#include <WinSock2.h>
#pragma comment(lib, "ws2_32");
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
    
    ret = connect(server_socket, (sockaddr *)&server_addr, sizeof(server_addr));
    if(ret == -1) {
        cout << "连接服务器失败！" << endl;
        WSACleanup();
	    return -1;
    }
    cout << "连接服务器成功！" << endl;

    string send_message;
    cout << "请输入要发送的数据: ";
    cin >> send_message;

    ret = send(server_socket, send_message.c_str(), send_message.size(), 0);
    if(ret == -1) {
        cout << "发送消息失败！" << endl;
        WSACleanup();
	    return -1;
    }
    cout << "发送消息成功！" << endl;

    const int BUFF_SIZE = 100;
    char buf[BUFF_SIZE];

    ret = recv(server_socket, buf, BUFF_SIZE, 0);
    if(ret <= 0) {
        cout << "接收消息失败！" << endl;
        WSACleanup();
	    return -1;
    }
    buf[ret] = '\0';
    cout << "Server: " << buf << endl;

    WSACleanup();

    return 0;
}
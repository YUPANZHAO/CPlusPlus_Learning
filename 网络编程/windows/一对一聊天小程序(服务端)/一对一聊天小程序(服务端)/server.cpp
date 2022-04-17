#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include <WinSock2.h>
#pragma comment(lib, "ws2_32")
#include <iostream>
#include <thread>
#include <Windows.h>
using namespace std;

SOCKET client_socket;
const int BUFF_SIZE = 10000;

void speak_func() {
    char buf[BUFF_SIZE];
    while (true) {
        cout << "Server: ";
        cin.getline(buf, BUFF_SIZE);
        int ret = send(client_socket, buf, strlen(buf), 0);
        if (ret == -1) {
            cout << "消息发送失败！" << endl;
            break;
        }
    }
}

void hear_func() {
    char buf[BUFF_SIZE];
    while (true) {
        int ret = recv(client_socket, buf, BUFF_SIZE, 0);
        if (ret <= 0) {
            cout << "消息接收失败！" << endl;
            break;
        }
        buf[ret] = '\0';
        COORD coord;
        HANDLE handler = GetStdHandle(STD_OUTPUT_HANDLE);
        CONSOLE_SCREEN_BUFFER_INFO bInfo;
        GetConsoleScreenBufferInfo(handler, &bInfo);
        coord = bInfo.dwCursorPosition;
        SetConsoleCursorPosition(handler, COORD{ 0,coord.Y });
        for (int i = 0; i < 100; ++i) cout << ' ';
        SetConsoleCursorPosition(handler, COORD{ 0,coord.Y });
        cout << "Client: " << buf << endl;
        cout << "Server: ";
    }
    cout << "客户端断开连接！" << endl;
}

int main() {

    WSADATA data;
    int ret = WSAStartup(MAKEWORD(2, 2), &data);
    if (ret) {
        cout << "初始化网络错误！" << endl;
        WSACleanup();
        return -1;
    }
    cout << "初始化网络成功！" << endl;

    SOCKET server_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (server_socket == -1) {
        cout << "创建套接字失败！" << endl;
        WSACleanup();
        return -1;
    }
    cout << "创建套接字成功！" << endl;

    int port;
    string host;
    cout << "请输入服务器IP: ";
    cin >> host;
    cin.get();
    cout << "请输入端口号: ";
    cin >> port;
    cin.get();

    sockaddr_in server_addr;
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(port);
    server_addr.sin_addr.S_un.S_addr = inet_addr(host.c_str());

    ret = bind(server_socket, (sockaddr*)&server_addr, sizeof(server_addr));
    if (ret == -1) {
        cout << "绑定地址端口失败！" << endl;
        WSACleanup();
        return -1;
    }
    cout << "绑定地址端口成功！" << endl;

    ret = listen(server_socket, 5);
    if (ret == -1) {
        cout << "监听套接字失败！" << endl;
        WSACleanup();
        return -1;
    }
    cout << "监听套接字成功！" << endl;

    sockaddr_in client_addr;
    int len = sizeof(client_addr);
    client_socket = accept(server_socket, (sockaddr*)&client_addr, &len);
    if (client_socket == -1) {
        cout << "接收客户端连接失败！" << endl;
        WSACleanup();
        return -1;
    }
    cout << "客户端连接成功" << endl;
    cout << endl;
    cout << "客户IP及端口号: " << inet_ntoa(client_addr.sin_addr) << ":" << ntohs(client_addr.sin_port) << endl;
    cout << "现开启通话..." << endl;
    cout << endl;

    thread speak(speak_func);
    thread hear(hear_func);

    speak.join();
    hear.join();

    WSACleanup();

    return 0;

}
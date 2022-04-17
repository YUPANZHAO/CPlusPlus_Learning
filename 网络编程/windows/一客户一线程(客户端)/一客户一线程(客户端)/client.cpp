#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include <WinSock2.h>
#pragma comment(lib, "ws2_32")
#include <iostream>
#include <thread>
#include <cstring>
using namespace std;

int main() {

    WSADATA data;
    int ret = WSAStartup(MAKEWORD(2, 2), &data);
    if (ret) {
        cout << "��ʼ���������" << endl;
        WSACleanup();
        return -1;
    }

    SOCKET server_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (server_socket == -1) {
        cout << "�����׽���ʧ�ܣ�" << endl;
        WSACleanup();
        return -1;
    }

    int port;
    string host;
    cout << "�����������IP: ";
    cin >> host;
    cin.get();
    cout << "������˿ں�: ";
    cin >> port;
    cin.get();

    sockaddr_in server_addr;
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(port);
    server_addr.sin_addr.S_un.S_addr = inet_addr(host.c_str());

    ret = connect(server_socket, (sockaddr*)&server_addr, sizeof(server_addr));
    if (ret == -1) {
        cout << "����������ʧ�ܣ�" << endl;
        WSACleanup();
        return -1;
    }
    cout << "���������ӳɹ���" << endl;

    const int BUFF_SIZE = 100;
    char buf[BUFF_SIZE];

    while (true) {
        cout << "������Ҫ���͵���Ϣ: ";
        cin.getline(buf, BUFF_SIZE);

        ret = send(server_socket, buf, strlen(buf), 0);
        if (ret == -1) {
            cout << "��Ϣ����ʧ�ܣ�" << endl;
            WSACleanup();
            return -1;
        }
        
        ret = recv(server_socket, buf, BUFF_SIZE, 0);
        if (ret <= 0) {
            cout << "��Ϣ����ʧ�ܣ�" << endl;
            WSACleanup();
            return -1;
        }

        buf[ret] = '\0';
        cout << "Server: " << buf << endl;
    }

    WSACleanup();

    return 0;

}
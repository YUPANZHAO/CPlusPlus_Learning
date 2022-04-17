#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include <WinSock2.h>
#pragma comment(lib, "ws2_32")
#include <iostream>
#include <thread>
#include "ClientThread.h"
using namespace std;

int main() {

	WSADATA data;
	int ret = WSAStartup(MAKEWORD(2, 2), &data);
    if (ret) {
        cout << "��ʼ���������" << endl;
        WSACleanup();
        return -1;
    }
    cout << "��ʼ������ɹ���" << endl;

    SOCKET server_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (server_socket == -1) {
        cout << "�����׽���ʧ�ܣ�" << endl;
        WSACleanup();
        return -1;
    }
    cout << "�����׽��ֳɹ���" << endl;

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

    ret = bind(server_socket, (sockaddr*)&server_addr, sizeof(server_addr));
    if (ret == -1) {
        cout << "�󶨵�ַ�˿�ʧ�ܣ�" << endl;
        WSACleanup();
        return -1;
    }
    cout << "�󶨵�ַ�˿ڳɹ���" << endl;

    ret = listen(server_socket, 5);
    if (ret == -1) {
        cout << "�����׽���ʧ�ܣ�" << endl;
        WSACleanup();
        return -1;
    }
    cout << "�����׽��ֳɹ���" << endl;

    while (true) {
        sockaddr_in client_addr;
        int len = sizeof(client_addr);
        SOCKET client_socket = accept(server_socket, (sockaddr*)&client_addr, &len);
        if (client_socket == -1) {
            cout << "���տͻ�������ʧ�ܣ�" << endl;
            WSACleanup();
            return -1;
        }

        ClientThread client_thread(client_socket, client_addr);
        client_thread.start();
    }

    WSACleanup();

	return 0;

}
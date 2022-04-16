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
        cout << "��ʼ���������" << endl;
        WSACleanup();
        return -1;
    }
    cout << "��ʼ������ɹ���" << endl;

    SOCKET server_socket = socket(AF_INET, SOCK_STREAM, 0);
    if(server_socket == -1) {
        cout << "�����׽���ʧ�ܣ�" << endl;
        WSACleanup();
        return -1;
    }
    cout << "�����׽��ֳɹ���" << endl;

    sockaddr_in server_addr;
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    server_addr.sin_addr.S_un.S_addr = inet_addr(HOST);
    
    ret = connect(server_socket, (sockaddr *)&server_addr, sizeof(server_addr));
    if(ret == -1) {
        cout << "���ӷ�����ʧ�ܣ�" << endl;
        WSACleanup();
	    return -1;
    }
    cout << "���ӷ������ɹ���" << endl;

    string send_message;
    cout << "������Ҫ���͵�����: ";
    cin >> send_message;

    ret = send(server_socket, send_message.c_str(), send_message.size(), 0);
    if(ret == -1) {
        cout << "������Ϣʧ�ܣ�" << endl;
        WSACleanup();
	    return -1;
    }
    cout << "������Ϣ�ɹ���" << endl;

    const int BUFF_SIZE = 100;
    char buf[BUFF_SIZE];

    ret = recv(server_socket, buf, BUFF_SIZE, 0);
    if(ret <= 0) {
        cout << "������Ϣʧ�ܣ�" << endl;
        WSACleanup();
	    return -1;
    }
    buf[ret] = '\0';
    cout << "Server: " << buf << endl;

    WSACleanup();

    return 0;
}
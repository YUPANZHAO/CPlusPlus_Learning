#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include <WinSock2.h>
#pragma comment(lib, "ws2_32")
#include <iostream>
#include <thread>
#include <Windows.h>
#include <queue>
using namespace std;

SOCKET server_socket;
string nick_name;
const int BUFF_SIZE = 1024;

void speak_func() {
	char buf[BUFF_SIZE];
	while (true) {
		cout << nick_name << ": ";
		cin.getline(buf, BUFF_SIZE-2);
		int len = strlen(buf);
		buf[len] = '\n';
		buf[len + 1] = '\0';
		int ret = send(server_socket, buf, len+1, 0);
		if (ret == -1) {
			cout << "��Ϣ����ʧ�ܣ�" << endl;
			break;
		}
	}
}

void hear_func() {
	char buf[BUFF_SIZE];
	while (true) {
		int ret = recv(server_socket, buf, BUFF_SIZE-1, 0);
		if (ret <= 0) {
			cout << "��Ϣ����ʧ�ܣ�" << endl;
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
		cout << buf;
		cout << nick_name << ": ";
	}
	cout << "����˶Ͽ����ӣ�" << endl;
}

int main() {
	WSADATA data;
	int ret = WSAStartup(MAKEWORD(2, 2), &data);
	if (ret) {
		cout << "��ʼ���������" << endl;
		WSACleanup();
		return -1;
	}

	server_socket = socket(AF_INET, SOCK_STREAM, 0);
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
		getchar();
		return -1;
	}
	cout << "���������ӳɹ���" << endl;

	char buf[10];
	int len = recv(server_socket, buf, 10, 0);
	if (len <= 0) {
		cout << "�������Ͽ����ӣ�" << endl;
		WSACleanup();
		getchar();
		return -1;
	}
	else if (buf[0] == 'E') {
		cout << "����������Ա��" << endl;
		WSACleanup();
		getchar();
		return -1;
	}
	cout << "���Ѿ��ɹ����������ң�" << endl;
	cout << "�����������ǳ�: ";
	cin >> nick_name;
	cin.get();
	
	ret = send(server_socket, nick_name.c_str(), nick_name.length(), 0);
	if (ret == -1) {
		cout << "�������Ͽ����ӣ�" << endl;
		WSACleanup();
		getchar();
		return -1;
	}
	
	thread speak(speak_func);
	thread hear(hear_func);
	speak.join();
	hear.join();

	closesocket(server_socket);
	WSACleanup();

	return 0;
}
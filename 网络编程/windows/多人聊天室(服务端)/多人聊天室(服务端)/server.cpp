#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include <WinSock2.h>
#pragma comment(lib, "ws2_32")
#include <iostream>
#include <thread>
#include <Windows.h>
#include <queue>
using namespace std;

const int CLIENT_NUM = 32;
const int BUFFER_SIZE = 1024;
const int MAX_CLIENT_NAME = 20;

int client_num = 0;

queue<string> message_que[CLIENT_NUM];

struct CLIENT_INFO {
	int valid;
	int id;
	SOCKET socket;
	char name[MAX_CLIENT_NAME + 1];
}client_info[CLIENT_NUM];

void handle_send(CLIENT_INFO* client) {
	string send_message;
	int tot, len, ret;
	while (client->valid) {
		while (!message_que[client->id].empty()) {
			send_message = message_que[client->id].front();
			message_que[client->id].pop();
			tot = send_message.length();
			len = tot > BUFFER_SIZE ? BUFFER_SIZE : tot;
			while (tot > 0) {
				ret = send(client->socket, send_message.c_str(), len, 0);
				if (ret == -1) {
					cout << "���ݷ���ʧ�ܣ�" << endl;
					return;
				}
				tot -= len;
				send_message.erase(0, len);
				len = tot > BUFFER_SIZE ? BUFFER_SIZE : tot;
			}
		}
	}
}

void handle_recv(CLIENT_INFO* client) {
	char buf[BUFFER_SIZE];
	int len;
	string recv_message = string(client->name) + ": ";
	while (true) {
		len = recv(client->socket, buf, BUFFER_SIZE, 0);
		if (len <= 0) {
			client->valid = 0;
			return;
		}
		else {
			for (int i = 0; i < len; ++i) {
				recv_message += buf[i];
				if (buf[i] == '\n') {
					for (int j = 0; j < CLIENT_NUM; ++j) {
						if (client_info[j].valid && client_info[j].socket != client->socket) {
							message_que[j].push(recv_message);
						}
					}
					recv_message = string(client->name) + ": ";
				}
			}
		}
	}
}

void chat(CLIENT_INFO* client) {
	cout << "�û� " + string(client->name) + " ����������" << endl;
	for (int i = 0; i < CLIENT_NUM; ++i) {
		if (client_info[i].valid && client_info[i].socket != client->socket) {
			message_que[i].push("�û� " + string(client->name) + " ����������\n");
		}
	}
	thread speak(handle_send, client);
	thread hear(handle_recv, client);
	speak.join();
	hear.join();
	shutdown(client->socket, 2);
	for (int i = 0; i < CLIENT_NUM; ++i) {
		if (client_info[i].valid && client_info[i].socket != client->socket) {
			message_que[i].push("�û� " + string(client->name) + " �˳�������\n");
		}
	}
	cout << "�û� " + string(client->name) + " �˳�������" << endl;
}

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

	cout << "�ȴ��û����ӷ�����..." << endl;

	while (true) {
		sockaddr_in client_addr;
		int len = sizeof(client_addr);
		SOCKET client_socket = accept(server_socket, (sockaddr*)&client_addr, &len);
		if (client_socket == -1) {
			cout << "���տͻ�������ʧ�ܣ�" << endl;
			WSACleanup();
			return -1;
		}
		if (client_num >= CLIENT_NUM) {
			send(client_socket, "ERROR", strlen("ERROR"), 0);
			shutdown(client_socket, 2);
			continue;
		}
		send(client_socket, "OK", strlen("OK"), 0);
		
		cout << "�û�IP���˿ں�: " << inet_ntoa(client_addr.sin_addr) << ":" << ntohs(client_addr.sin_port) << endl;
		++client_num;
		for (int i = 0; i < CLIENT_NUM; ++i) {
			if (!client_info[i].valid) {
				int len = recv(client_socket, client_info[i].name, MAX_CLIENT_NAME + 1, 0);
				if (len <= 0) {
					cout << "�û��뿪" << endl;
					--client_num;
					break;
				}
				client_info[i].name[len] = '\0';

				while (!message_que[i].empty()) message_que[i].pop();
				client_info[i].id = i;
				client_info[i].valid = 1;
				client_info[i].socket = client_socket;

				thread thrd(chat, &client_info[i]);
				thrd.detach();

				break;
			}
		}
	}

	closesocket(server_socket);
	WSACleanup();

	return 0;
}
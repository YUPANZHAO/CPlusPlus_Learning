#pragma once
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include <iostream>
#include <WinSock2.h>
#pragma comment(lib, "ws2_32")
#include <thread>

const int BUFF_SIZE = 100;
class ClientThread
{
private:
	SOCKET client_socket;
	sockaddr_in client_addr;
	char buf[BUFF_SIZE];
public:
	ClientThread();
	ClientThread(SOCKET client_socket, sockaddr_in client_addr);
	void operator() ();
	void start();
};


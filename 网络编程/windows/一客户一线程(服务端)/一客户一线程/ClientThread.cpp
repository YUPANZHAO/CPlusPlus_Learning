#include "ClientThread.h"

ClientThread::ClientThread() {}

ClientThread::ClientThread(SOCKET client_socket, sockaddr_in client_addr) {
	this->client_socket = client_socket;
	this->client_addr = client_addr;
}

void ClientThread::operator() () {
	while (true) {
		int len = recv(client_socket, buf, BUFF_SIZE, 0);
		if (len <= 0) break;
		buf[len] = '\0';
		std::cout << "Client(" << inet_ntoa(client_addr.sin_addr) << ":" << ntohs(client_addr.sin_port) << "): " << buf << std::endl;

		int ret = send(client_socket, buf, len, 0);
		if (ret == -1) {
			std::cout << "消息回传失败" << std::endl;
			return ;
		}
		std::cout << "消息回传成功" << std::endl;
	}
	std::cout << "客户 " << inet_ntoa(client_addr.sin_addr) << ":" << ntohs(client_addr.sin_port) << " 退出连接" << std::endl;
}

void ClientThread::start() {
	std::cout << "客户 " << inet_ntoa(client_addr.sin_addr) << ":" << ntohs(client_addr.sin_port) << " 已连接" << std::endl;
	std::thread thrd(*this);
	thrd.detach();
}
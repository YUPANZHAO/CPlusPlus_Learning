#include "MyServer.h"

void MyServer::flushError() {
    error_message = string(strerror(errno)); 
}

MyServer::~MyServer() {
    shutdown(server_socket, 2);
    memset(&server_addr, 0, sizeof(server_addr));
}

bool MyServer::Listen(int n) {
    server_socket = socket(AF_INET, SOCK_STREAM, 0);
    if(server_socket == -1) {
        flushError();
        return false;
    }
    
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(port);
    server_addr.sin_addr.s_addr = inet_addr(host.c_str());
    
    if(bind(server_socket, (sockaddr *)&server_addr, sizeof(server_addr)) == -1) {
        flushError();
        return false;
    }

    if(listen(server_socket, n) == -1) {
        flushError();
        return false;
    }
    return true;
}

MyServer::ClientInfo MyServer::Accept() {
    ClientInfo client;
    client.addr_len = sizeof(client.addr);
    client.socket = accept(server_socket, (sockaddr *)&client.addr, &client.addr_len);
    if(client.socket == -1) {
        flushError();
    }
    return client;
}

int MyServer::Send(ClientInfo client, string message) {
    char buf[32];
    int len = message.length();
    int temp = len;
    int res = 0;
    for(int i=0; i < 32; ++i) {
        if(temp&1) buf[i] = '1';
        else buf[i] = '0';
        temp >>= 1;
    }
    if(send(client.socket, buf, 32, 0) == -1) {
        flushError();
        return -1;
    }
    int n = len > BUFFER_SIZE ? BUFFER_SIZE : len;
    while(len) {
        temp = send(client.socket, message.c_str(), n, 0);
        if(temp == -1) {
            flushError();
            return -1;
        }
        res += temp;
        message.erase(0, temp);
        len -= temp;
        n = len > BUFFER_SIZE ? BUFFER_SIZE : len;
    }
    return res;
}

string MyServer::Recv(ClientInfo client) {
    string res = "";
    char buf[BUFFER_SIZE];
    if(recv(client.socket, buf, 32, 0) <= 0) {
        flushError();
        return NULL;
    }
    int len = 0;
    for(int i=31; i >= 0; --i) {
        len <<= 1;
        if(buf[i] == '1') len |= 1;
    }  
    int n = len > BUFFER_SIZE ? BUFFER_SIZE : len;
    while(len) {
        int temp = recv(client.socket, buf, n, 0);
        if(temp <= 0) {
            flushError();
            return NULL;
        }
        for(int i=0; i < temp; ++i)
            res += buf[i];
        len -= temp;
        n = len > BUFFER_SIZE ? BUFFER_SIZE : len;
    }
    return res;
}

string MyServer::Error() {
    return error_message;
}

void MyServer::ShutDown(ClientInfo client) {
    shutdown(client.socket, 2);
}
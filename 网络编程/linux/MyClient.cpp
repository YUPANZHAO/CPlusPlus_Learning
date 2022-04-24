#include "MyClient.h"

void MyClient::flushError() {
    error_message = string(strerror(errno)); 
}

MyClient::~MyClient() {
    shutdown(server_socket, 2);
    memset(&server_addr, 0, sizeof(server_addr));
}

bool MyClient::Connect() {
    server_socket = socket(AF_INET, SOCK_STREAM, 0);
    if(server_socket == -1) {
        flushError();
        return false;
    }
    
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(port);
    server_addr.sin_addr.s_addr = inet_addr(host.c_str());
    
    if(connect(server_socket, (sockaddr *)&server_addr, sizeof(server_addr)) == -1) {
        flushError();
        return false;
    }
    return true;
}

int MyClient::Send(string message) {
    char buf[32];
    int len = message.length();
    int temp = len;
    int res = 0;
    for(int i=0; i < 32; ++i) {
        if(temp&1) buf[i] = '1';
        else buf[i] = '0';
        temp >>= 1;
    }
    if(send(server_socket, buf, 32, 0) == -1) {
        flushError();
        return -1;
    }
    int n = len > BUFFER_SIZE ? BUFFER_SIZE : len;
    while(len) {
        temp = send(server_socket, message.c_str(), n, 0);
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

string MyClient::Recv() {
    string res = "";
    char buf[BUFFER_SIZE];
    if(recv(server_socket, buf, 32, 0) <= 0) {
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
        int temp = recv(server_socket, buf, n, 0);
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

string MyClient::Error() {
    return error_message;
}
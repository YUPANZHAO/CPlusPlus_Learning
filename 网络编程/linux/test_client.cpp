#include <iostream>
#include <cstring>
#include "MyClient.h"
using namespace std;

MyClient client;

int main() {
    if(client.Connect() == false) {
        cout << "连接服务端失败" << endl;
        cout << "错误消息：" << client.Error() << endl;
        return -1;
    }else {
        cout << "服务端连接成功" << endl;
    }
    
    cout << "请输入要发送的消息：";
    string mes;
    getline(cin, mes);
    
    client.Send(mes);
    
    string res = client.Recv();
    
    cout << "服务端回传消息：" << res << endl;
}
/*
    #include <signal.h>

    int sigprocmask(int how, const sigset_t *set, sigset_t *oldset);
        功能：获取或修改内核中的阻塞信号集
        参数：
            how：如何对内核阻塞信号集mask进行处理
                SIG_BLOCK : 将用户设置的阻塞信号集添加到内核中，内核中原来的数据不变（mask |= set）
                SIG_UNBLOCK : 根据用户设置的数据，对内核中的数据进行解除阻塞（mask &= ~set）
                SIG_SETMASK : 将用户设置的阻塞信号集完全替换内核数据（mask = set）
            set：已经初始化好的阻塞信号集
            oldset：传出参数，保存设置前的内核阻塞信号集
        返回值：
            成功 0
            失败 -1
                设置错误号 EFAULT、EINVAL

    int sigpending(sigset_t *set);
        功能：获取内核中的未决信号集
        参数：传出参数，保证内核中的未决信号集
        返回值：成功 0，失败 -1
*/

#include <signal.h>
#include <iostream>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

using namespace std;

int main() {

    // 创建信号集合
    sigset_t set;

    // 清空信号集
    sigemptyset(&set);

    // 添加2，3信号
    sigaddset(&set, SIGINT);
    sigaddset(&set, SIGQUIT);

    // 修改内核中的阻塞信号集
    sigprocmask(SIG_BLOCK, &set, NULL);

    int num = 0;

    while(true) {
        num++;
        // 获取当前的未决信号集数据
        sigset_t pending_set;
        sigemptyset(&pending_set);
        sigpending(&pending_set);
        // 遍历前32位
        for(int i=1; i <= 32; ++i) {
            int ret = sigismember(&pending_set, i);
            if(ret != -1) cout << ret;
            else {
                perror("sigismember");
                exit(0);
            }
        }
        cout << endl;
        sleep(1);
        
        if(num == 10) {
            // 解除阻塞
            sigprocmask(SIG_UNBLOCK, &set, NULL);
        }

    }

    return 0;
}
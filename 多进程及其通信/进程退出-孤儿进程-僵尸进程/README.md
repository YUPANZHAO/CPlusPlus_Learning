## 进程退出

```cpp
#include <stdlib.h>
int exit(int status);

#include <unistd.h>
int _exit(int status);
```

exit()是标准C库的函数，在执行时，会刷新I/O缓冲区，并关闭文件描述符，然后再调用_exit()终止进程

而_exit()并不会刷新I/O缓冲区

两个函数传入的参数status，是进程退出的一个状态信息，父进程回收子进程资源的时候可以获取到

## 孤儿进程

父进程运行结束，而子进程还在运行，这样的子进程被称为**孤儿进程**

每当出现一个孤儿进程时，内核会把孤儿进程的父进程设置为init

init进程会循环地wait()子进程的退出

所以孤儿进程并不会有什么危害

## 僵尸进程

每个进程执行结束，都会释放自己地址空间中的用户数据，内核区的PCB没有办法自己释放掉，需要父进程去释放

进程终止时，父进程尚未回收，子进程残留资源(PCB)存放于内核中，变成**僵尸进程(Zombie)**

僵尸进程不能被 `kill -9` 杀死

这就会导致一个问题，如果父进程不调用wait()或waitpid()的话，那么保留的那段信息就不会释放，其进程号就会一直占用，但是系统所能使用的进程号是有限的，如果大量的产生僵尸进程，将因为没有可用的进程号而导致系统不能产生新的进程，此即为僵尸进程的危害，应当避免

**解决僵尸进程的办法**，可以通过捕捉子进程结束时向父进程发送的SIGCHLD信号，再利用waitpid进行回收操作即可。详细代码见: [sigchld.cpp](../信号相关/sigchld.cpp)

## wait()和waitpid()

父进程可以调用wait()或waitpid()得到子进程的退出状态，同时彻底清除掉子进程

wait()和waitpid()函数功能一样，区别在于wait()会阻塞，waitpid()可以设置非阻塞，waitpid()还可以指定等待哪个进程结束

PS：一次wait()或waitpid()调用只能清理一个子进程，清理多个子进程应使用循环

## 退出信息相关宏函数

```cpp
WIFEXITED(status)   // 非0，进程正常退出
WEXITSTATUS(status) // 如果上面宏为真，获取进程退出的状态(exit的参数)

WIFSIGNALED(status) // 非0，进程异常终止
WTERMSIG(status)    // 如果上面宏为真，获取使进程终止的信号编号

WIFSTOPPED(status)  // 非0，进程处于暂停状态
WSTOPSIG(status)    // 如果上面宏为真，获取使进程暂停的信号编号
WIFCONTINUED(status)// 非0，进程暂停后已经继续运行
```
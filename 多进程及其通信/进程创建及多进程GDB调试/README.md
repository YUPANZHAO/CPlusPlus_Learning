## 父子进程虚拟地址空间的情况

创建子进程的时候，内核会创建新的虚拟地址空间给子进程

其内核区中的数据会有不同，但用户区的数据在一开始是一样的

实际上，Linux的fork()会通过 **写时拷贝技术(copy-on-write)** 来推迟甚至避免用户区的数据拷贝

其原理是通过让父子进程共享同一个地址空间实现的

只有在资源的执行写操作的时候才会进行拷贝操作

## GDB调试多进程

使用GDB调试的时候，GDB默认只能跟踪一个进程

可以在fork函数调用之前，通过指令设置GDB调试工具跟踪父进程或子进程，默认跟踪父进程

查看默认调试的进程

    show follow-fork-mode

设置调试父进程或者子进程

    set follow-fork-mode [ parent | child ]

设置调试模式

    set detach-on-fork [ on | off ]
    默认为on，表示调试当前进程时，其他进程会脱离GDB的控制，其他进程会继续运行
    如果为off，调试当前进程时，其他进程会被挂起

查看调试的进程

    info inferiors
    
切换当前调试的进程

    inferiors id
    此id为'info inferiors'查找出的进程的编号，而非进程pid

使进程脱离GDB的控制

    detach inferiors id
    此id为'info inferiors'查找出的进程的编号，而非进程pid
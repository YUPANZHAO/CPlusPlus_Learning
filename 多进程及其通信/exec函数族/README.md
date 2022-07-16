## exec函数族

exec函数族的作用是根据指定的文件名找到可执行文件，并用它来取代调用进程的内容

exec函数族执行成功后不会返回，因为调用进程的实体，包括代码段、数据段、堆栈等都被新的内容取代

仅在调用失败的情况下才会返回-1，并设置errno

``` cpp
int execl(const char *path, const char *arg, ...);

int execlp(const char *file, const char *arg, ...);

int execle(const char *path, const char *arg, ..., char * const envp[]);

int execv(const char *path, char *const argv[]);

int execvp(const char *file, char *const argv[]);

int execvpe(const char *file, char *const argv[], char *const envp[]);

int execve(const char *filename, char *const argv[], char *const envp[]);
```

| 函数名后缀 | 全称 | 含义 |
| --- | --- | --- |
| l | list | 参数地址列表，以空指针结尾 |
| v | vector | 存有各参数地址的指针数组的地址 |
| p | path | 按PATH环境变量指定目录搜索可执行文件 |
| e | environment | 存有环境变量字符串地址的指针数组的地址 |
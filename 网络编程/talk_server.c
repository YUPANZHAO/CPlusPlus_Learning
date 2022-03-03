#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <string.h>
#include <pthread.h>
#include <ctype.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <errno.h>

#define SERVER_PORT 666

int server_fd, client_fd, res;
struct sockaddr_in server_addr;

pthread_t say;
pthread_t hear;

void* say_something() {
    int len;
    char buf[100];
    while(1) {
        scanf("%s", buf);
	len = strlen(buf);
	write(client_fd, buf, len);
	printf("S: %s\n", buf);
        if(!strcmp(buf, "exit")) {
            pthread_cancel(hear);
            break;
        }
    }
}

void* hear_something() {
    int len;
    char buf[100];
    while(1) {
        len = read(client_fd, buf, sizeof(buf)-1);
        buf[len] = '\0';
        if(len > 0) printf("C: %s\n", buf);
	if(!strcmp(buf, "exit")) {
	    pthread_cancel(say);
	    break;
	}
    }
}

void error_happen() {
    perror(strerror(errno));
    exit(1);
}

int main() {
    
    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    
    if(server_fd == -1) error_happen();
 
    bzero(&server_addr, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(SERVER_PORT);
    server_addr.sin_addr.s_addr = htonl(INADDR_ANY);

    res = bind(server_fd, (struct sockaddr*)&server_addr, sizeof(server_addr));

    if(res == -1) error_happen();

    res = listen(server_fd, 128);
    
    if(res == -1) error_happen();

    printf("服务端已开启\n");
    
    while(1) {

        client_fd = accept(server_fd, NULL, NULL);
        printf("客户端已连接\n");
        
        pthread_create(&say, NULL, say_something, NULL);
        pthread_create(&hear, NULL, hear_something, NULL);

        pthread_join(say, NULL);
	pthread_join(hear, NULL);
 
        printf("断开连接\n");
    }
    
    return 0;
}

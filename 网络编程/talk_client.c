#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <string.h>
#include <pthread.h>
#include <ctype.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <errno.h>

#define SERVER_PORT 666
#define SERVER_IP "192.168.71.2"

int client_fd, res;
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
	printf("C: %s\n", buf);
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
        if(len > 0) printf("S: %s\n", buf);
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
    
    client_fd = socket(AF_INET, SOCK_STREAM, 0);
    
    if(client_fd == -1) error_happen();

    bzero(&server_addr, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(SERVER_PORT);
    inet_pton(AF_INET, SERVER_IP, &server_addr.sin_addr);

    int res = connect(client_fd, (struct sockaddr*)&server_addr, sizeof(server_addr));
    
    if(res == -1) error_happen();

    printf("服务端已连接\n");
    
    pthread_create(&say, NULL, say_something, NULL);
    pthread_create(&hear, NULL, hear_something, NULL);

    pthread_join(say, NULL);
    pthread_join(hear, NULL);
    
    printf("断开连接\n");
    
    return 0;
}

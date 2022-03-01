#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <string.h>
#include <ctype.h>
#include <arpa/inet.h>

#define SERVER_PORT 666

int main() {
    int sock;
    struct sockaddr_in server_addr;
    
    sock = socket(AF_INET, SOCK_STREAM, 0);    
    
    bzero(&server_addr, sizeof(server_addr));
    
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    server_addr.sin_port = htons(SERVER_PORT);

    bind(sock, (struct sockaddr *)&server_addr, sizeof(server_addr));

    listen(sock, 128);

    printf("等待客户端的连接\n");

    int done = 1;
    while(done) {
	struct sockaddr_in client;
	int client_sock, len;
	char client_ip[64];
	char buf[256];	
	
	socklen_t client_addr_len;
	client_addr_len = sizeof(client);
	client_sock = accept(sock, (struct sockaddr *)&client, &client_addr_len);
	
	printf("client ip: %s\tport:%d\n",
		inet_ntop(AF_INET, &client.sin_addr.s_addr, client_ip, sizeof(client_ip)),
		ntohs(client.sin_port));
	
	len = read(client_sock, buf, sizeof(buf)-1);
	buf[len] = '\0';
	printf("recive[%d]: %s\n", len, buf);

	int i;
	for(i=0; i < len; i++) buf[i] = toupper(buf[i]);

	len = write(client_sock, buf, len);
	
	printf("write finished, len=%d\n", len);
	close(client_sock);	    
    }

    return 0;
}

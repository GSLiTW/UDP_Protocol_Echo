#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define BUF_SIZE 1024

int main(int argc, char ** argv){
    char *ip = argv[1];
    int port = atoi(argv[2]);

    int sockfd;
    struct sockaddr_in server_addr, client_addr;
    char buffer[BUF_SIZE];
    socklen_t addr_size;
    int n;

    if(argc != 3){
        printf("Usage: %s <IP> <PORT>\n", argv[0]);
        exit(1);
    }

    sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if(sockfd < 0){
        perror("socket error");
        exit(1);
    }

    memset(&server_addr, '\0', sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(port);
    server_addr.sin_addr.s_addr = inet_addr(ip);

    n = bind(sockfd, (struct sockaddr*)&server_addr, sizeof(server_addr));
    if(n < 0){
        perror("bind error");
        exit(1);
    }

    bzero(buffer, BUF_SIZE);
    addr_size = sizeof(client_addr);
    recvfrom(sockfd, buffer, BUF_SIZE, 0, (struct sockaddr*)&client_addr, &addr_size);
    sendto(sockfd, buffer, BUF_SIZE, 0, (struct sockaddr*)&client_addr, addr_size);

    return 0;
}
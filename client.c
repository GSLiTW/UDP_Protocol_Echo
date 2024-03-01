#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define BUF_SIZE 1024
#define MAX_WAIT_INTERVAL 8000 // in milliseconds

int main(int argc, char ** argv){
    char *ip = argv[1];
    int port = atoi(argv[2]);
    char *message = argv[3];
    int message_len = strlen(message);
    int max_retry = atoi(argv[4]);
    int base_timeout = 500; 

    int sockfd;
    struct sockaddr_in server_addr;
    char buffer[BUF_SIZE];
    socklen_t addr_size;
    int n;
    int count = 0;

    if(argc != 5){
        printf("Usage: %s <IP> <PORT> <Message> <Max_Retry>\n", argv[0]);
        exit(1);
    }

    sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if(sockfd < 0){
        perror("Socket error");
        exit(1);
    }

    memset(&server_addr, '\0', sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(port);
    server_addr.sin_addr.s_addr = inet_addr(ip);



    bzero(buffer, BUF_SIZE);
    strcpy(buffer, message);
    addr_size = sizeof(server_addr);
    sendto(sockfd, buffer, message_len, 0, (struct sockaddr*)&server_addr, addr_size);
    bzero(buffer, 1024);
    recvfrom(sockfd, buffer, BUF_SIZE, 0, (struct sockaddr*)&server_addr, &addr_size);
    printf("Received echo: %s\n", buffer);

    return 0;
}
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>

int main(int argc, char ** argv){
    if(argc != 4){
        printf("Should pass in: IP PORT Message\n");
        exit(1);
    }

    char *ip = argv[1];
    int port = atoi(argv[2]);
    char *message = argv[3];

    int sockfd;
    struct sockaddr_in server_addr;
    char buffer[1024];
    socklen_t addr_size;
    int n;

    sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if(sockfd < 0){
        perror("Socket error");
        exit(1);
    }

    memset(&server_addr, '\0', sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(port);
    server_addr.sin_addr.s_addr = inet_addr(ip);

    bzero(buffer, 1024);
    strcpy(buffer, message);
    addr_size = sizeof(server_addr);
    sendto(sockfd, buffer, 1024, 0, (struct sockaddr*)&server_addr, addr_size);
    bzero(buffer, 1024);
    recvfrom(sockfd, buffer, 1024, 0, (struct sockaddr*)&server_addr, &addr_size);
    printf("Received echo: %s\n", buffer);

    return 0;
}
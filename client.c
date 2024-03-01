#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <sys/time.h>

#define BUF_SIZE 1024
#define MAX_WAIT_INTERVAL 8000 // in milliseconds

int main(int argc, char ** argv){
    if(argc != 5){
        printf("Usage: %s <IP> <PORT> <Message> <Max_Retry>\n", argv[0]);
        exit(1);
    }

    char *ip = argv[1];
    int port = atoi(argv[2]);
    char *message = argv[3];
    int message_len = strlen(message);
    int max_retry = atoi(argv[4]);

    int sockfd;
    struct sockaddr_in server_addr;
    char buffer[BUF_SIZE];
    socklen_t addr_size;
    int n;

    int base_timeout = 500; // in milliseconds
    int count = 0;
    int current_timeout = base_timeout;
    struct timeval timeout_value;

    sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if(sockfd < 0){
        perror("Socket error");
        exit(1);
    }

    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(port);
    server_addr.sin_addr.s_addr = inet_addr(ip);

    fd_set read_fds;
    FD_ZERO(&read_fds);
    FD_SET(sockfd, &read_fds);
    while(count < max_retry){
        current_timeout = base_timeout * (1 << count); // timeout = 2^count
        if (current_timeout > MAX_WAIT_INTERVAL) {
            current_timeout = MAX_WAIT_INTERVAL;
        }

        timeout_value.tv_sec = current_timeout / 1000;
        timeout_value.tv_usec = (current_timeout % 1000) * 1000;

        addr_size = sizeof(server_addr);
        sendto(sockfd, message, message_len, 0, (struct sockaddr*)&server_addr, addr_size);

        // Wait for response or timeout
        int activity = select(sockfd + 1, &read_fds, NULL, NULL, &timeout_value);

        if (activity < 0) {
            perror("Select error");
            exit(1);
        }
        else if (activity == 0) {
            printf("Timeout occurred, timeout interval: %d ms, retrying...\n", current_timeout);
            count++;
            continue;
        }
        else {
            bzero(buffer, BUF_SIZE);
            recvfrom(sockfd, buffer, BUF_SIZE, 0, (struct sockaddr*)&server_addr, &addr_size);
            printf("Received echo: %s\n", buffer);
            exit(0);
        }
    }

    printf("No echo received, exiting with 1.\n");
    exit(1); // Going to this step means no echo returned.
}
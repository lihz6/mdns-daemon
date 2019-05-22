#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <stdio.h>

#define BUFFERSIZE 2048

void print_buffer(const char *buffer, size_t size)
{
    for (size_t i = 0; i < size; i++)
    {
        printf("%02X", buffer[i]);
        if (i % 2)
        {
            putchar('\n');
        }
        else
        {
            putchar(' ');
        }
    }
}

int main(void)
{
    int sockfd;
    char buffer[BUFFERSIZE];
    char *query_message = "\xAA\xAA"
                          "\x01\x00"
                          "\x00\x01"
                          "\x00\x00"
                          "\x00\x00"
                          "\x00\x00"
                          "\x04\x6c"
                          "\x69\x68"
                          "\x7a\x05"
                          "\x6c\x6f"
                          "\x63\x61"
                          "\x6c\x00"
                          "\x00\x01"
                          "\x00\x01";
    struct sockaddr_in sock_addr;
    socklen_t addr_len;
    ssize_t readsize;
    if (0 > (sockfd = socket(AF_INET, SOCK_DGRAM, 0)))
    {
        perror("socket creation failed");
        exit(EXIT_FAILURE);
    }

    memset(&sock_addr, 0, sizeof(struct sockaddr_in));

    // Filling server information
    sock_addr.sin_family = AF_INET;
    sock_addr.sin_port = htons(5353);
    sock_addr.sin_addr.s_addr = INADDR_ANY;
    readsize = 28;
    if (readsize != sendto(sockfd, query_message, readsize, 0, (struct sockaddr *)&sock_addr, sizeof(sock_addr)))
    {
        perror("sendto failed");
        exit(EXIT_FAILURE);
    }

    printf("Hello message sent.\n");

    readsize = recvfrom(sockfd, (char *)buffer, BUFFERSIZE, 0, (struct sockaddr *)&sock_addr, &addr_len);
    if (0 > readsize)
    {
        perror("recvfrom failed");
        exit(EXIT_FAILURE);
    }

    print_buffer(buffer, readsize);

    close(sockfd);
}

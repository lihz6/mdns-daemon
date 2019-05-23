#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <stdio.h>

#define BUFFERSIZE 2048

void print_buffer(const unsigned char *buffer, size_t size)
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
    unsigned char buffer[BUFFERSIZE];
    // python3: ''.join(['\\' + hex(ord(c))[1:] for c in 'hostname'])
    unsigned char query_message[] = "\xAA\xAA"
                                    "\x01\x00"
                                    "\x00\x01"
                                    "\x00\x00"
                                    "\x00\x00"
                                    "\x00\x00"
                                    "\x06\x6c\x61\x77\x79\x6f\x6f\x05\x6c\x6f\x63\x61\x6c\x00"
                                    "\x00\x01"
                                    "\x00\x01";
    struct sockaddr_in sock_addr;
    // struct ip_mreq mreq;
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
    // mreq.imr_multiaddr.s_addr = inet_addr("224.0.0.251");
    // mreq.imr_interface.s_addr = htonl(INADDR_ANY);
    // if (0 > setsockopt(sockfd, IPPROTO_IP, IP_ADD_MEMBERSHIP, &mreq, sizeof(mreq)))
    // {
    //     perror("join failed");
    //     exit(EXIT_FAILURE);
    // }
    readsize = sizeof(query_message) - 1, addr_len = sizeof(struct sockaddr_in);
    if (readsize != sendto(sockfd, query_message, readsize, 0, (struct sockaddr *)&sock_addr, addr_len))
    {
        perror("sendto failed");
        exit(EXIT_FAILURE);
    }

    printf("Hello message sent.\n");

    readsize = recvfrom(sockfd, buffer, BUFFERSIZE, 0, (struct sockaddr *)&sock_addr, &addr_len);
    if (0 > readsize)
    {
        perror("recvfrom failed");
        exit(EXIT_FAILURE);
    }

    print_buffer(buffer, readsize);

    close(sockfd);
}
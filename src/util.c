#include <sys/socket.h>
#include <arpa/inet.h>
#include <stddef.h>
#include <string.h>
#include <stdio.h>
#include "util.h"

void print_buffer(const unsigned char *buffer, size_t size)
{
    size_t i;
    for (i = 0; i < size; i++)
    {
        printf(" %02X", buffer[i]);
        if (i % 2)
        {
            putchar('\n');
        }
    }
    putchar('\n');
}

int open_socket()
{
    int sockfd;
    size_t enabled = 1;
    struct sockaddr_in bind_addr;
    struct ip_mreq mreq;

    if (0 > (sockfd = socket(AF_INET, SOCK_DGRAM, 0)))
    {
        fprintf(stderr, "Could not open\n");
        return -1;
    }
    if (0 > setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &enabled, sizeof(size_t)))
    {
        fprintf(stderr, "Reuseaddr failed\n");
        return -1;
    }
#ifdef SO_REUSEPORT
    if (0 > setsockopt(sockfd, SOL_SOCKET, SO_REUSEPORT, &enabled, sizeof(size_t)))
    {
        fprintf(stderr, "Reuseport failed\n");
        return -1;
    }
#endif
    memset(&bind_addr, 0, sizeof(struct sockaddr_in));
    bind_addr.sin_family = AF_INET;
    bind_addr.sin_port = htons(5353);
    bind_addr.sin_addr.s_addr = INADDR_ANY;
    if (0 > bind(sockfd, (struct sockaddr *)&bind_addr, sizeof(struct sockaddr_in)))
    {
        fprintf(stderr, "Could not bind\n");
        return -1;
    }
    // TODO: join IPv6 membership
    mreq.imr_multiaddr.s_addr = inet_addr("224.0.0.251");
    mreq.imr_interface.s_addr = htonl(INADDR_ANY);
    if (0 > setsockopt(sockfd, IPPROTO_IP, IP_ADD_MEMBERSHIP, &mreq, sizeof(mreq)))
    {
        fprintf(stderr, "Could not join\n");
        return -1;
    }
    return sockfd;
}
#include <sys/socket.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <stdio.h>
#include <netdb.h>
#include <errno.h>
#include "util.h"

#define BUFFERSIZE 2048

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
    mreq.imr_multiaddr.s_addr = inet_addr("224.0.0.251");
    mreq.imr_interface.s_addr = htonl(INADDR_ANY);
    if (0 > setsockopt(sockfd, IPPROTO_IP, IP_ADD_MEMBERSHIP, &mreq, sizeof(mreq)))
    {
        fprintf(stderr, "Could not join\n");
        return -1;
    }
    return sockfd;
}

void print_buffer(const unsigned char *buffer, size_t size)
{
    for (size_t i = 0; i < size; i++)
    {
        printf(" %02X", buffer[i]);
        if (i % 2)
        {
            putchar('\n');
        }
    }
}

int main(void)
{
    int sockfd, s;
    struct sockaddr_storage peer_addr;
    socklen_t addr_len;
    ssize_t readsize;
    unsigned char buffer[BUFFERSIZE];
    char peer_ip[INET6_ADDRSTRLEN];
    struct dns_header_t *dns_header;
    if (0 > (sockfd = open_socket()))
    {
        fprintf(stderr, "Could not open\n");
        exit(EXIT_FAILURE);
    }

    for (;;)
    {
        addr_len = sizeof(struct sockaddr_storage);
        readsize = recvfrom(sockfd, buffer, BUFFERSIZE, 0, (struct sockaddr *)&peer_addr, &addr_len);
        if (0 > readsize)
        {
            continue;
        }

        pull_dns_header(buffer, &dns_header);

        if (dns_header->DNSFLAG & DNSFLAG_RESPD_MESSAGE_BIT)
        {
            printf("Got a response message\n");
            continue; // No interests in response messages
        }

        if (peer_addr.ss_family == AF_INET)
        {
            struct sockaddr_in *s = (struct sockaddr_in *)&peer_addr;
            inet_ntop(AF_INET, &s->sin_addr, peer_ip, sizeof peer_ip);
        }
        else
        {
            struct sockaddr_in6 *s = (struct sockaddr_in6 *)&peer_addr;
            inet_ntop(AF_INET6, &s->sin6_addr, peer_ip, sizeof peer_ip);
        }
        printf("Got %zd bytes query message from %s\n", readsize, peer_ip);

        print_buffer(buffer, readsize);

        if (sendto(sockfd, buffer, readsize, 0, (struct sockaddr *)&peer_addr, addr_len) != readsize)
        {
            fprintf(stderr, "Error sending response\n");
        }
    }
}
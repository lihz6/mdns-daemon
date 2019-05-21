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

#define BUFFER_SIZE 2048

int open_socket()
{
    int sfd;
    size_t enabled = 1;
    struct sockaddr_in addr;
    struct ip_mreq mreq;

    if (0 > (sfd = socket(AF_INET, SOCK_DGRAM, 0)))
    {
        fprintf(stderr, "Could not open\n");
        return -1;
    }
    if (0 > setsockopt(sfd, SOL_SOCKET, SO_REUSEADDR, &enabled, sizeof(size_t)))
    {
        fprintf(stderr, "Reuseaddr failed\n");
        return -1;
    }
#ifdef SO_REUSEPORT
    if (0 > setsockopt(sfd, SOL_SOCKET, SO_REUSEPORT, &enabled, sizeof(size_t)))
    {
        fprintf(stderr, "Reuseport failed\n");
        return -1;
    }
#endif
    memset(&addr, 0, sizeof(struct sockaddr_in));
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = htonl(INADDR_ANY);
    addr.sin_port = htons(5353);
    if (0 > bind(sfd, (struct sockaddr *)&addr, sizeof(struct sockaddr_in)))
    {
        fprintf(stderr, "Could not bind\n");
        return -1;
    }
    mreq.imr_multiaddr.s_addr = inet_addr("224.0.0.251");
    mreq.imr_interface.s_addr = htonl(INADDR_ANY);
    if (0 > setsockopt(sfd, IPPROTO_IP, IP_ADD_MEMBERSHIP, &mreq, sizeof(mreq)))
    {
        fprintf(stderr, "Could not join\n");
        return -1;
    }
    return sfd;
}

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
    int sfd, s;
    struct sockaddr_storage peer_addr;
    socklen_t peer_addr_len;
    ssize_t nread;
    unsigned char buffer[BUFFER_SIZE];
    struct dns_header_t *dns_header;
    if (0 > (sfd = open_socket()))
    {
        fprintf(stderr, "Could not open\n");
        exit(EXIT_FAILURE);
    }

    for (;;)
    {
        peer_addr_len = sizeof(struct sockaddr_storage);
        nread = recvfrom(sfd, buffer, BUFFER_SIZE, 0, (struct sockaddr *)&peer_addr, &peer_addr_len);
        if (0 > nread)
        {
            continue;
        }
        pull_dns_header(buffer, &dns_header);
        if (dns_header->DNSFLAG & DNSFLAG_QUERY == DNSFLAG_QUERY)
        {
            printf("This is query message\n");
        }
        {
            printf("This is one question\n");
        }
        print_buffer(buffer, nread);

        char host[NI_MAXHOST], service[NI_MAXSERV];
        s = getnameinfo((struct sockaddr *)&peer_addr, peer_addr_len, host, NI_MAXHOST, service, NI_MAXSERV, NI_NUMERICSERV);
        if (s == 0)
        {
            printf("Received %zd bytes from %s:%s\n", nread, host, service);
        }
        else
        {
            fprintf(stderr, "getnameinfo: %s\n", gai_strerror(s));
        }

        if (sendto(sfd, buffer, nread, 0, (struct sockaddr *)&peer_addr, peer_addr_len) != nread)
        {
            fprintf(stderr, "Error sending response\n");
        }
    }
}
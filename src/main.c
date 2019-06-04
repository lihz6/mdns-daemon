#include <sys/socket.h>
#include <sys/types.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <stdio.h>
#include <netdb.h>
#include <errno.h>
#include "mdns.h"
#include "host.h"
#include "ipad.h"
#include "util.h"
#define BUFFERSIZE 4096
#define PACKETSIZE 512

int main(void)
{
    // about socket
    int sockfd;
    // about peer socket
    struct sockaddr_storage peer_addr;
    // struct sockaddr_in6 *peer_addr6;
    struct sockaddr_in *peer_addr4;
    uint32_t uint_ipv4;
    // char peer_ip[INET6_ADDRSTRLEN];
    socklen_t peer_slen;
    // about buffer
    unsigned char hostlist[BUFFERSIZE];
    char hostname[NI_MAXHOST];
    unsigned char *buffer, *puffer;
    size_t buffersize = 0;
    ssize_t iosize;
    struct dns_header_t *dns_header;
    struct answer_t *answer;
    // about dns header
    // open socket
    if (0 > (sockfd = open_socket()))
    {
        fprintf(stderr, "Could not open\n");
        exit(EXIT_FAILURE);
    }
    // query all available hostnames
    loadup_hostlist(hostlist, BUFFERSIZE - PACKETSIZE);
    printf("Respnod for:\n");
    while (hostlist[buffersize])
    {
        printf("  %s\n", hostlist + buffersize + 1);
        buffersize += hostlist[buffersize];
    }
    buffer = hostlist + buffersize + 1;
    // wait for messages
    for (;;)
    {
        peer_slen = sizeof(struct sockaddr_storage);
        iosize = recvfrom(sockfd, buffer, PACKETSIZE, 0, (struct sockaddr *)&peer_addr, &peer_slen);
        if (0 > iosize || peer_addr.ss_family != AF_INET)
        {
            // TODO: join IPv6 membership
            printf("Got IPv6 message\n");
            continue;
        }
        // parse dns header
        dns_header = (struct dns_header_t *)buffer;

        if (IS_DNSFLAG_RESPD(dns_header->DNSFLAG) || 0 == dns_header->QDCOUNT)
        {
            pull_hostname(buffer + 12, hostname);
            if (!strstr(hostname, "._"))
            {
                printf("\n\nGot respn for %s\n", hostname);
                print_buffer(buffer, iosize);
            }
            continue;
        }
        pull_hostname(buffer + 12, hostname);
        if (!strstr(hostname, "._"))
        {
            printf("\n\nGot query for %s\n", hostname);
            print_buffer(buffer, iosize);
        }
        puffer = buffer + sizeof(struct dns_header_t);
        if (offset_hostname(puffer))
        {
            // TODO: answer questions when QDCOUNT > 1
            continue;
        }
        puffer = pull_hostname(puffer, hostname);
        answer = (struct answer_t *)puffer;
        if (hostname[0] == '_' || answer->TYPE != TYPE_A || !lookup_hostname(hostlist, hostname))
        {
            // TODO: answer IPv6 query
            // printf("Not me: %s\n", hostname);
            continue;
        }
        dns_header->DNSFLAG = DNSFLAG_RESPD_NO_ORROR;
        dns_header->QDCOUNT = 0;
        dns_header->ANCOUNT = ENCOUNT(1);
        dns_header->NSCOUNT = 0;
        dns_header->ARCOUNT = 0;
        // answer->CLASS = ACLASS_IN;
        answer->TTL = TTL_4_HOURS;
        answer->RDLENGTH = RDLENGTH_IPv4;
        peer_addr4 = (struct sockaddr_in *)&peer_addr;
        uint_ipv4 = htonl(peer_addr4->sin_addr.s_addr);
        uint_ipv4 = lookup_ipv4(uint_ipv4);
        *(uint32_t *)(puffer + sizeof(struct answer_t)) = htonl(uint_ipv4);
        iosize = puffer - buffer + sizeof(struct answer_t) + sizeof(uint32_t);
        sendto(sockfd, buffer, iosize, 0, (struct sockaddr *)&peer_addr, peer_slen);
        printf("\n\nSend to %s\n", hostname);
        print_buffer(buffer, iosize);
    }
}

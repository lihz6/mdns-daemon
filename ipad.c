#include <arpa/inet.h>
#include <stdbool.h>
#include <string.h>
#include <unistd.h>
#include <stdint.h>
#include <netdb.h>
#include <stdio.h>

uint32_t lookup_ipv4(uint32_t peerip)
{
    char host[NI_MAXHOST], addr[INET_ADDRSTRLEN], *h_addr4;
    uint32_t ipaddr, flagip = 0;
    struct hostent *hent4;
    gethostname(host, NI_MAXHOST);
    hent4 = gethostbyname2(host, AF_INET);
    while (h_addr4 = *hent4->h_addr_list++)
    {
        ipaddr = ntohl(*(uint32_t *)h_addr4);
        if ((ipaddr ^ peerip) < (flagip ^ peerip))
        {
            flagip = ipaddr;
        }
    }
    return flagip;
}
#include <arpa/inet.h>
#include <stdbool.h>
#include <string.h>
#include <unistd.h>
#include <stdint.h>
#include <netdb.h>
#include <stdio.h>
#include "ipad.h"
#include "mdns.h"

ipv4_t lookup_ipv4(ipv4_t peerip)
{
    char host[NI_MAXHOST], *h_addr4;
    ipv4_t ipaddr, flagip = ~peerip;
    struct hostent *hent4;
    gethostname(host, NI_MAXHOST);
    hent4 = gethostbyname2(host, AF_INET);
    while ((h_addr4 = *hent4->h_addr_list++))
    {
        ipaddr = ntohl(*(ipv4_t *)h_addr4);
        if ((ipaddr ^ peerip) < (flagip ^ peerip))
        {
            flagip = ipaddr;
        }
    }
    return flagip;
}
char *lookup_ipv6()
{
    char host[NI_MAXHOST], *h_addr6;
    struct hostent *hent6;
    gethostname(host, NI_MAXHOST);
    hent6 = gethostbyname2(host, AF_INET6);
    while ((h_addr6 = *hent6->h_addr_list++))
    {
        return h_addr6;
    }
    return NULL;
}

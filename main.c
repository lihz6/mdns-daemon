#include <arpa/inet.h>
#include <stdbool.h>
#include <string.h>
#include <stdio.h>
#include <netdb.h>
#include <unistd.h>

void query_ips(void)
{
    char host[NI_MAXHOST], addr[INET6_ADDRSTRLEN], *h_addr4, *h_addr6;
    gethostname(host, NI_MAXHOST);
    printf("hostname: %s\n", host);
    struct hostent *hent4, *hent6;
    hent4 = gethostbyname2(host, AF_INET);
    // hent6 = gethostbyname2(host, AF_INET6);
    while ((h_addr4 = *hent4->h_addr_list++) /* && (h_addr6 = *hent6->h_addr_list++)*/)
    {
        inet_ntop(hent4->h_addrtype, h_addr4, addr, INET6_ADDRSTRLEN);
        printf("h_addr: %s\n", addr);
        uint32_t *ipnum = (uint32_t *)h_addr4;
        printf("ipnum: %x\n", ntohl(*ipnum));
        // inet_ntop(hent6->h_addrtype, h_addr6, addr, INET6_ADDRSTRLEN);
        // printf("h_addr: %s\n", addr);
    }
}

int main(void)
{
    query_ips();
}

// read hosts
// get hostip
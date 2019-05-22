#include <arpa/inet.h>
#include <stdbool.h>
#include <string.h>
#include <stdio.h>
#include <netdb.h>

void query_ips(int af)
{
    char host[NI_MAXHOST], addr[INET6_ADDRSTRLEN], *h_addr;
    gethostname(host, NI_MAXHOST);
    struct hostent *hent;
    hent = gethostbyname2(host, af);
    while (h_addr = *hent->h_addr_list++)
    {
        inet_ntop(hent->h_addrtype, h_addr, addr, INET6_ADDRSTRLEN);
        printf("h_addr: %s\n", addr);
    }
}

int main(void)
{
    query_ips(AF_INET);
    query_ips(AF_INET6);
}

// read hosts
// get hostip
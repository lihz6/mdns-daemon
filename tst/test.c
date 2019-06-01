#include <sys/param.h>
#include <arpa/inet.h>
#include <stdbool.h>
#include <string.h>
#include <unistd.h>
#include <stdio.h>
#include <netdb.h>
#include "../src/host.h"

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
void void_test_hostlist(void)
{
    char buffer[2048], *puffer;
    // puffer = buffer;
    // puffer = encode_hostname(puffer, "abc", 128);
    // puffer = encode_hostname(puffer, "abc", 128);
    // puffer = encode_hostname(puffer, "abc", 128);
    // *puffer = '\0';
    size_t start = 0;
    global_hostlist(buffer, 2048);
    while (buffer[start])
    {
        printf("hostname: %s\n", buffer + start + 1);
        start += buffer[start];
    }
    if (lookup_hostname(buffer, "vmubuntu.local"))
    {
        printf("FOUND: vmubuntu.local\n");
    }
    if (lookup_hostname(buffer, "vmubuntu.vm.local"))
    {
        printf("FOUND: vmubuntu.vm.local\n");
    }
    if (lookup_hostname(buffer, "vmvmubuntu-vm.local"))
    {
        printf("FOUND: vmvmubuntu-vm.local\n");
    }
    if (lookup_hostname(buffer, "vm.vmubuntu.vmos.local"))
    {
        printf("FOUND: vm.vmubuntu.vmos.local\n");
    }
}

int main(void)
{
    uint32_t a = -1;
    printf("%x\n", a);
}
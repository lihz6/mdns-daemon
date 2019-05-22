#include <arpa/inet.h>
#include <stdbool.h>
#include <string.h>
#include <stdio.h>
#include <netdb.h>

#include <unistd.h>

bool valid_hostname(const char *hostname)
{
    if (strstr(hostname, "localhost") || strstr(hostname, "loopback") || strstr(hostname, "localdomain"))
    {
        return false;
    }
    for (;;)
    {
        switch (*hostname++)
        {
        case '\0':
            return false;
        case '.':
            return true;
        }
    }
}

// read `/etc/hosts` file
void etc_hosts(void)
{
    char addr[INET_ADDRSTRLEN], *h_name;
    struct hostent *hent;
    inet_pton(AF_INET, "127.0.0.1", addr);
    sethostent(true);
    while (hent = gethostent())
    {
        if (hent->h_addrtype != AF_INET || strcmp(hent->h_addr, addr))
        {
            continue;
        }
        h_name = hent->h_name;
        do
        {
            if (valid_hostname(h_name))
            {
                printf("h_name: %s\n", h_name);
            }
        } while (h_name = *hent->h_aliases++);
    }
    endhostent();
}

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
    etc_hosts();
    query_ips(AF_INET);
    query_ips(AF_INET6);
}

// read hosts
// get hostip
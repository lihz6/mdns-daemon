#include <sys/param.h>
#include <arpa/inet.h>
#include <stdbool.h>
#include <stddef.h>
#include <unistd.h>
#include <string.h>
#include <netdb.h>
#include "host.h"

bool is_global_hostname(const char *hostname)
{
    if (strstr(hostname, "localhost") || strstr(hostname, "loopback") || strstr(hostname, "localdomain"))
    {
        return false;
    }
    return strchr(hostname, '.') != NULL;
}

char *encode_hostname(char *hostlist, const char *hostname, size_t lentotal)
{
    char *position = hostlist++;
    while (lentotal--)
    {
        if (!(*hostlist++ = *hostname++))
        {
            *position = hostlist - position;
            return hostlist;
        }
    }
    return position;
}

void global_hostlist(char *hostlist, size_t hostlist_len)
{
    const char *pinpoint = hostlist + hostlist_len - 2;
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
            if (is_global_hostname(h_name))
            {
                hostlist = encode_hostname(hostlist, h_name, MIN(pinpoint - hostlist, 128));
            }
        } while (h_name = *hent->h_aliases++);
    }
    *hostlist = '\0';
    endhostent();
}

bool lookup_hostname(const char *const hostlist, const char *const hostname)
{
    size_t start, end = 0;
    const char *hoststop;
    while (hostlist[end])
    {
        start = end + 1;
        end += hostlist[end];
        if (strcmp(hostlist + start, hostname) == 0)
        {
            return true;
        }
        hoststop = hostname;
        while (strchr(".-", hostlist[start]) && (hoststop = strpbrk(hoststop + 1, ".-")))
        {
            if (strcmp(hostlist + start++, hoststop) == 0)
            {
                return true;
            }
        }
    }

    return false;
}
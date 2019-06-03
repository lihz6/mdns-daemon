#include <sys/param.h>
#include <arpa/inet.h>
#include <stdbool.h>
#include <stddef.h>
#include <unistd.h>
#include <string.h>
#include <limits.h>
#include <netdb.h>
#include "host.h"

bool is_global_hostname(const char *hostname)
{
    return hostname[0] != '_' && strchr(hostname, '.') != NULL && strstr(hostname, "localhost") == NULL && strstr(hostname, "loopback") == NULL && strstr(hostname, "localdomain") == NULL;
}

unsigned char *encode_hostname(unsigned char *hostlist, const char *hostname, size_t limitlen)
{
    unsigned char *position = hostlist++;
    while (--limitlen)
    {
        if (!(*hostlist++ = *hostname++))
        {
            limitlen = hostlist - position;
            if (limitlen > UCHAR_MAX)
            {
                return position;
            }
            *position = limitlen;
            return hostlist;
        }
    }
    return position;
}

void global_hostlist(unsigned char *hostlist, const size_t listsize)
{
    const unsigned char *pinpoint = hostlist + listsize - 1;
    char addr[INET_ADDRSTRLEN], *h_name;
    struct hostent *hent;
    inet_pton(AF_INET, "127.0.0.1", addr);
    sethostent(true);
    while ((hent = gethostent()))
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
        } while ((h_name = *hent->h_aliases++));
    }
    endhostent();
    if ((size_t)(pinpoint - hostlist) == listsize - 1)
    {
        gethostname((char *)(hostlist + 1), listsize - 1);
        hostlist = encode_hostname(hostlist, strcat((char *)(hostlist + 1), ".local"), MIN(pinpoint - hostlist, 128));
    }
    *hostlist = '\0';
}

bool lookup_hostname(const unsigned char *const hostlist, const char *const hostname)
{
    size_t start, end = 0;
    const char *hoststop;
    while (hostlist[end])
    {
        start = end + 1;
        end += hostlist[end];
        if (strcmp((const char *)(hostlist + start), hostname) == 0)
        {
            return true;
        }
        hoststop = hostname;
        while (strchr(".-", hostlist[start]) && (hoststop = strpbrk(hoststop + 1, ".-")))
        {
            if (strcmp((const char *)(hostlist + start++), hoststop) == 0)
            {
                return true;
            }
        }
    }

    return false;
}
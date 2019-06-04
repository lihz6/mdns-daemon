#include <sys/param.h>
#include <arpa/inet.h>
#include <stdbool.h>
#include <stddef.h>
#include <unistd.h>
#include <string.h>
#include <limits.h>
#include <netdb.h>
#include "host.h"

static bool is_hostname(const char *hostname)
{
    return hostname[0] != '_'                   // can't start with `_`
           && strchr(hostname, '.')             // must contain `.`
           && !strstr(hostname, "loopback")     // can't contain `loopback`
           && !strstr(hostname, "localhost")    // can't contain `localhost`
           && !strstr(hostname, "localdomain"); // can't contain `localdomain`
}

// hostname: aaaa.bbb.cc\0
// hostlist: \13aaaa.bbb.cc\0*
static unsigned char *encode_hostname(unsigned char *hostlist, const char *hostname, size_t limitlen)
{
    unsigned char *position = hostlist++;
    while (--limitlen)
    {
        if (!(*hostlist++ = *hostname++))
        {
            *position = hostlist - position;
            return hostlist;
        }
    }
    return position;
}

// hostlist: \5aaaa\0\4aaa\0\3aa\0\2a\0\0
void loadup_hostlist(unsigned char *hostlist, const size_t listsize)
{
    const unsigned char *pinpoint = hostlist + listsize /* reserved 1 byte for the last `\0` */ - 1;
    char addr[INET_ADDRSTRLEN], *h_name;
    struct hostent *hent;
    inet_pton(AF_INET, "127.0.0.1", addr);
    // read from `/etc/hosts@[127.0.0.1]`
    sethostent(true);
    while ((hent = gethostent()))
    {
        if (AF_INET == hent->h_addrtype && 0 == strcmp(hent->h_addr, addr))
        {
            h_name = hent->h_name;
            do
            {
                if (is_hostname(h_name))
                {
                    hostlist = encode_hostname(hostlist, h_name, MIN(pinpoint - hostlist, UCHAR_MAX));
                }
            } while ((h_name = *hent->h_aliases++));
        }
    }
    endhostent();
    // default loadup <hostname>.local
    if (pinpoint == hostlist + listsize - 1)
    {
        gethostname((char *)(hostlist + 1), listsize - 1);
        hostlist = encode_hostname(hostlist, strcat((char *)(hostlist + 1), ".local"), MIN(listsize - 1, UCHAR_MAX));
    }
    // the last `\0`
    *hostlist = '\0';
}

// look up hostname in hostlist
bool lookup_hostname(const unsigned char *const hostlist, const char *const hostname)
{
    size_t start, end = 0;
    const char *hoststop;
    while (hostlist[end])
    {
        start = end + 1;
        end += hostlist[end];
        if (strcmp((char *)(hostlist + start), hostname) == 0)
        {
            return true;
        }
        hoststop = hostname;
        while (strchr(".-", hostlist[start]) && (hoststop = strpbrk(hoststop + 1, ".-")))
        {
            if (strcmp((char *)(hostlist + start++), hoststop) == 0)
            {
                return true;
            }
        }
    }

    return false;
}
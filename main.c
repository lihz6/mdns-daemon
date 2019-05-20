#include <arpa/inet.h>
#include <stdbool.h>
#include <string.h>
#include <stdio.h>
#include <netdb.h>

#define LOCALHOST "127.0.0.1"
#define DOT_LOCAL ".local"

// read `/etc/hosts` file
int main(void)
{
    char addr[INET_ADDRSTRLEN], **h_list, *h_name;
    struct hostent *hent;
    size_t hlen;
    inet_pton(AF_INET, LOCALHOST, addr);
    sethostent(true);
    while (hent = gethostent())
    {
        if (hent->h_addrtype != AF_INET || strcmp(hent->h_addr, addr))
        {
            continue;
        }
        h_name = hent->h_name;
        h_list = hent->h_aliases;
        do
        {
            hlen = strlen(h_name) - strlen(DOT_LOCAL);
            if (hlen > 0 && strcmp(h_name + hlen, DOT_LOCAL) == 0)
            {
                printf("h_name: %s\n", h_name);
            }
        } while (h_name = *h_list++);
    }
    endhostent();
}

#include <arpa/inet.h> // `htonl`, `htons`, `ntohl`, `ntohs`
#include <unistd.h>    // `getpid`
#include <string.h>
#include <stdio.h>
#include "mdns.h"

unsigned char *push_dns_header(unsigned char *buffer)
{
    struct dns_header_t *dns_header = (struct dns_header_t *)buffer;
    dns_header->ID = 0;
    dns_header->DNSFLAG = DNSFLAG_QUERY;
    dns_header->QDCOUNT = ENCOUNT(1);
    dns_header->ANCOUNT = 0;
    dns_header->NSCOUNT = 0;
    dns_header->ARCOUNT = 0;
    return buffer + sizeof(struct dns_header_t);
}

uint16_t offset_hostname(const unsigned char *buffer)
{
    uint16_t offset = ntohs(*(uint16_t *)buffer);
    if ((offset & 0xC000) == 0xC000)
    {
        return offset & 0x3FFF;
    }
    return 0;
}

unsigned char *push_hostname(unsigned char *buffer, const char *hostname)
{
    unsigned char *position = buffer++;
    for (;;)
    {
        switch (*buffer++ = *hostname++)
        {
        case '\0':
            *position = buffer - position - 2;
            return buffer;
        case '.':
            *position = buffer - position - 2;
            position = buffer - 1;
        }
    }
}

unsigned char *pull_hostname(unsigned char *buffer, char *hostname)
{
    for (;;)
    {
        unsigned char length = *buffer++;
        while (length--)
        {
            *hostname++ = *buffer++;
        }
        if (*buffer)
        {
            *hostname++ = '.';
        }
        else
        {
            *hostname = '\0';
            return buffer + 1;
        }
    }
}

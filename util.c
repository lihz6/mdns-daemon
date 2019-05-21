#include <arpa/inet.h> // `htonl`, `htons`, `ntohl`, `ntohs`
#include <unistd.h>    // `getpid`
#include <string.h>
#include <stdio.h>
#include "util.h"

unsigned char *init_dns_header(unsigned char *buffer)
{
    struct dns_header_t *dns_header = (struct dns_header_t *)buffer;
    dns_header->ID = getpid();
    /**
     * 0: this is a query message
     * 1: this is a response message
     */
    // dns_header->QR = 0;
    // dns_header->OPCODE = 0;

    // dns_header->AA = 0;
    // dns_header->TC = 0;
    // dns_header->RD = 1;
    // dns_header->RA = 0;
    // dns_header->Z = 0;
    /**
     * of responses:
     *   0: No error
     *   1: Format error
     *   2: Server failure
     *   3: Name error
     *   4: Not Implemented
     *   5: Refused
     */
    // dns_header->RCODE = 0;
    dns_header->DNSFLAG = DNSFLAG_QUERY;
    /**
     * questions count
     */
    dns_header->QDCOUNT = XXCOUNT(1);
    // dns_header->QDCOUNT = 2019;
    /**
     * answers count
     */
    dns_header->ANCOUNT = 0;
    dns_header->NSCOUNT = 0;
    dns_header->ARCOUNT = 0;
    return buffer + sizeof(struct dns_header_t);
}

unsigned char *push_dns_header(unsigned char *buffer, const struct dns_header_t *dns_header)
{
    memcpy(buffer, dns_header, sizeof(struct dns_header_t));
    return buffer + sizeof(struct dns_header_t);
}

const unsigned char *pull_dns_header(const unsigned char *buffer, struct dns_header_t **dns_header)
{
    *dns_header = (struct dns_header_t *)buffer;
    return buffer + sizeof(struct dns_header_t);
}

size_t offset_hostname(const unsigned char *buffer)
{
    struct hostname_scheme_t *scheme = (struct hostname_scheme_t *)buffer;
    if (scheme->label == 0)
    {
        return 0;
    }
    return scheme->offset;
}

unsigned char *push_hostname(unsigned char *buffer, const unsigned char *hostname)
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

const unsigned char *pull_hostname(const unsigned char *buffer, unsigned char *hostname)
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

extern unsigned char *push_question(unsigned char *buffer, uint16_t QTYPE)
{
    struct question_t *question = (struct question_t *)buffer;
    /**
     *  1: IPv4
     * 28: IPv6
     */
    question->QTYPE = htons(QTYPE);
    question->QCLASS = htons(1);
    return buffer + sizeof(struct question_t);
}
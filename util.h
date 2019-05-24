#ifndef _MDNS_UTIL_H
#define _MDNS_UTIL_H
#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

#define DNSFLAG_QUERY htons(0x0100)
#define DNSFLAG_RESPD_NO_ORROR htons(0x8400)
#define DNSFLAG_RESPD_FORMAT_ORROR htons(0x8401)
#define DNSFLAG_RESPD_SERVER_ORROR htons(0x8402)
#define DNSFLAG_RESPD_NAME_ORROR htons(0x8403)
#define DNSFLAG_RESPD_NOT_IMPLEMENTED htons(0x8404)
#define DNSFLAG_RESPD_REFUSED htons(0x8405)

#define DNSFLAG_RESPD_MESSAGE_BIT htons(0x8000)

#define ENCOUNT(count) htons(count)
#define DECOUNT(count) ntohs(count)

#define QTYPE_A htons(0x0001)

// DNS header structure
// #pragma pack(push, 1)
struct dns_header_t
{
    uint16_t ID;
    // unsigned char QR : 1;
    // unsigned char OPCODE : 4;
    // unsigned char AA : 1;
    // unsigned char TC : 1;
    // unsigned char RD : 1;
    // unsigned char RA : 1;
    // unsigned char Z : 3;
    // unsigned char RCODE : 4;
    uint16_t DNSFLAG;
    uint16_t QDCOUNT; // number of question entries
    uint16_t ANCOUNT; // number of answer entries
    uint16_t NSCOUNT; // number of authority entries
    uint16_t ARCOUNT; // number of resource entries
};
struct question_t
{
    uint16_t QTYPE;
    uint16_t QCLASS;
};

struct answer_t
{
    uint16_t TYPE;
    uint16_t CLASS;
    uint32_t TTL;
    uint16_t RDLENGTH;
};
// #pragma(pop)

extern unsigned char *init_dns_header(unsigned char *buffer);

extern unsigned char *push_dns_header(unsigned char *buffer, const struct dns_header_t *dns_header);

// +--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+
// | 1  1|                OFFSET                   |
// +--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+
extern size_t offset_hostname(const unsigned char *buffer);

// "www.ngolin.com" -> "\3www\6ngolin\3com"
extern unsigned char *push_hostname(unsigned char *buffer, const unsigned char *hostname);

// "\3www\6ngolin\3com"  -> "www.ngolin.com"
extern const unsigned char *pull_hostname(const unsigned char *buffer, unsigned char *hostname);

extern unsigned char *push_question(unsigned char *buffer, uint16_t QTYPE);
#endif
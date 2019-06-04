#ifndef __MDNS_H
#define __MDNS_H
#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

// DNS header structure
// #pragma pack(push, 1)
struct dns_header_t
{
    // default 0
    uint16_t ID;
    // unsigned char QR : 1;
    // unsigned char OPCODE : 4;
    // unsigned char AA : 1;
    // unsigned char TC : 1;
    // unsigned char RD : 1;
    // unsigned char RA : 1;
    // unsigned char Z : 3;
    // unsigned char RCODE : 4;
#define DNSFLAG_QUERY htons(0x0100)
#define DNSFLAG_RESPD_NO_ORROR htons(0x8400)
#define DNSFLAG_RESPD_FORMAT_ORROR htons(0x8401)
#define DNSFLAG_RESPD_SERVER_ORROR htons(0x8402)
#define DNSFLAG_RESPD_NAME_ORROR htons(0x8403)
#define DNSFLAG_RESPD_NOT_IMPLEMENTED htons(0x8404)
#define DNSFLAG_RESPD_REFUSED htons(0x8405)
#define IS_DNSFLAG_QUERY(x) (x & htons(0x0100))
#define IS_DNSFLAG_RESPD(x) (x & htons(0x8000))
    uint16_t DNSFLAG;

#define ENCOUNT(count) htons(count)
#define DECOUNT(count) ntohs(count)
    uint16_t QDCOUNT; // number of question entries
    uint16_t ANCOUNT; // number of answer entries
    uint16_t NSCOUNT; // number of authority entries
    uint16_t ARCOUNT; // number of resource entries
};

struct answer_t
{
#define TYPE_AAAA htons(0x001C)
#define TYPE_A htons(0x0001)
    uint16_t TYPE;
#define QCLASS_IN htons(0x0001)
#define ACLASS_IN htons(0x8001)
    uint16_t CLASS;
#define TTL_4_HOURS htonl(0x3840)
    uint32_t TTL;
#define RDLENGTH_IPv4 htons(0x0004)
#define RDLENGTH_IPv6 htons(0x0010)
    uint16_t RDLENGTH;
};
// #pragma(pop)

extern unsigned char *push_dns_header(unsigned char *buffer);

// +--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+
// | 1  1|                OFFSET                   |
// +--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+
extern uint16_t offset_hostname(const unsigned char *buffer);

// "www.ngolin.com" -> "\3www\6ngolin\3com"
extern unsigned char *push_hostname(unsigned char *buffer, const char *hostname);

// "\3www\6ngolin\3com"  -> "www.ngolin.com"
extern unsigned char *pull_hostname(unsigned char *buffer, char *hostname);
#endif
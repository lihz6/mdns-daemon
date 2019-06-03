#include <stdbool.h>
#include <stddef.h>
extern unsigned char *encode_hostname(unsigned char *hostlist, const char *hostname, size_t limitlen);
extern void global_hostlist(unsigned char *hostlist, const size_t listsize);
extern bool lookup_hostname(const unsigned char *const hostlist, const char *const hostname);
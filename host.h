#include <stdbool.h>
#include <stddef.h>
extern char *encode_hostname(char *hostlist, const char *hostname, size_t limitlen);
extern void global_hostlist(char *hostlist, const size_t listsize);
extern bool lookup_hostname(const char *const hostlist, const char *const hostname);
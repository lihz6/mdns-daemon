#include <stdbool.h>
#include <stddef.h>
extern char *encode_hostname(char *hostlist, const char *hostname, size_t lentotal);
extern void global_hostlist(char *hostlist, size_t hostlist_len);
extern bool lookup_hostname(const char *const hostlist, const char *const hostname);
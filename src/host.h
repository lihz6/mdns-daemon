#ifndef __HOST_H
#define __HOST_H
#include <stdbool.h>
#include <stddef.h>

extern void loadup_hostlist(unsigned char *hostlist, const size_t listsize);
extern bool lookup_hostname(const unsigned char *const hostlist, const char *const hostname);
#endif
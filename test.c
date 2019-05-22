#include <sys/param.h>
#include <string.h>
#include <stdio.h>
#include "host.h"

int main(void)
{
    char buffer[2048], *puffer;
    // puffer = buffer;
    // puffer = encode_hostname(puffer, "abc", 128);
    // puffer = encode_hostname(puffer, "abc", 128);
    // puffer = encode_hostname(puffer, "abc", 128);
    // *puffer = '\0';
    size_t start = 0;
    global_hostlist(buffer, 2048);
    while (buffer[start])
    {
        printf("hostname: %s\n", buffer + start + 1);
        start += buffer[start];
    }
    if (lookup_hostname(buffer, "vmubuntu.local"))
    {
        printf("FOUND: vmubuntu.local\n");
    }
    if (lookup_hostname(buffer, "vmubuntu.vm.local"))
    {
        printf("FOUND: vmubuntu.vm.local\n");
    }
    if (lookup_hostname(buffer, "vmvmubuntu-vm.local"))
    {
        printf("FOUND: vmvmubuntu-vm.local\n");
    }
    if (lookup_hostname(buffer, "vm.vmubuntu.vmos.local"))
    {
        printf("FOUND: vm.vmubuntu.vmos.local\n");
    }
}
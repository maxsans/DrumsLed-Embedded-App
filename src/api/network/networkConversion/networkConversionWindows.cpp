/**
 * @file networkConversionWindows.cpp
 * @author Cyprien Ménard
 * @date 2025-08-22
 */

#include "networkConversion.hpp"
#include <winsock2.h>
#include <ws2tcpip.h>

uint32_t network_htonl(uint32_t hostlong)
{
    return htonl(hostlong);
}

uint16_t network_htons(uint16_t hostshort)
{
    return htons(hostshort);
}

uint32_t network_ntohl(uint32_t netlong)
{
    return ntohl(netlong);
}

uint16_t network_ntohs(uint16_t netshort)
{
    return ntohs(netshort);
}

uint64_t network_htonll(uint64_t hostlonglong)
{
    return ((uint64_t)htonl((uint32_t)(hostlonglong & 0xFFFFFFFF)) << 32)
           | htonl((uint32_t)(hostlonglong >> 32));
}

uint64_t network_ntohll(uint64_t netlonglong)
{
    return ((uint64_t)ntohl((uint32_t)(netlonglong & 0xFFFFFFFF)) << 32)
           | ntohl((uint32_t)(netlonglong >> 32));
}

uint32_t network_inet_addr(const char *cp)
{
    return inet_addr(cp);
}

const char *network_inet_ntoa(uint32_t addr)
{
    struct in_addr inaddr;
    inaddr.s_addr = addr;
    return inet_ntoa(inaddr);
}

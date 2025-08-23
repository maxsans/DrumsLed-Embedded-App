/**
 * @file networkConversionEsp8266.cpp
 * @author Cyprien Ménard
 * @date 2025-08-22
 */

#include "lwip/def.h"
#include "lwip/inet.h"
#include "networkConversion.hpp"

uint32_t network_htonl(uint32_t hostlong)
{
    return lwip_htonl(hostlong);
}

uint16_t network_htons(uint16_t hostshort)
{
    return lwip_htons(hostshort);
}

uint32_t network_ntohl(uint32_t netlong)
{
    return lwip_ntohl(netlong);
}

uint16_t network_ntohs(uint16_t netshort)
{
    return lwip_ntohs(netshort);
}

uint64_t network_htonll(uint64_t hostlonglong)
{
    return ((uint64_t)lwip_htonl((uint32_t)(hostlonglong & 0xFFFFFFFF)) << 32)
           | lwip_htonl((uint32_t)(hostlonglong >> 32));
}

uint64_t network_ntohll(uint64_t netlonglong)
{
    return ((uint64_t)lwip_ntohl((uint32_t)(netlonglong & 0xFFFFFFFF)) << 32)
           | lwip_ntohl((uint32_t)(netlonglong >> 32));
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

#include "ipv4.h"

IPv4::IPv4()
{
    m_ip[0] = 0;
    m_ip[1] = 0;
    m_ip[2] = 0;
    m_ip[3] = 0;
}

IPv4::IPv4(uint8_t ip1, uint8_t ip2, uint8_t ip3, uint8_t ip4)
{
    m_ip[0] = ip1;
    m_ip[1] = ip2;
    m_ip[2] = ip3;
    m_ip[3] = ip4;
}

IPv4::IPv4(uint32_t ip)
{
    m_ip[0] = (ip >> 24) & 0xFF;
    m_ip[1] = (ip >> 16) & 0xFF;
    m_ip[2] = (ip >> 8) & 0xFF;
    m_ip[3] = ip & 0xFF;
}

IPv4::IPv4(IPv4 &ip)
{
    m_ip[0] = ip.m_ip[0];
    m_ip[1] = ip.m_ip[1];
    m_ip[2] = ip.m_ip[2];
    m_ip[3] = ip.m_ip[3];
}

IPv4::IPv4(char *ip)
{
    setIp(ip);
}

IPv4::~IPv4()
{
}

uint8_t IPv4::getIp1()
{
    return m_ip[0];
}

uint8_t IPv4::getIp2()
{
    return m_ip[1];
}

uint8_t IPv4::getIp3()
{
    return m_ip[2];
}

uint8_t IPv4::getIp4()
{
    return m_ip[3];
}

uint32_t IPv4::getIp()
{
    return (m_ip[0] << 24) | (m_ip[1] << 16) | (m_ip[2] << 8) | m_ip[3];
}

std::string IPv4::getIpString()
{
    return std::to_string(m_ip[0]) + "." + std::to_string(m_ip[1]) + "." + std::to_string(m_ip[2]) + "." + std::to_string(m_ip[3]);
}

void IPv4::setIp1(uint8_t ip1)
{
    m_ip[0] = ip1;
}

void IPv4::setIp2(uint8_t ip2)
{
    m_ip[1] = ip2;
}

void IPv4::setIp3(uint8_t ip3)
{
    m_ip[2] = ip3;
}

void IPv4::setIp4(uint8_t ip4)
{
    m_ip[3] = ip4;
}

void IPv4::setIp(uint32_t ip)
{
    m_ip[0] = (ip >> 24) & 0xFF;
    m_ip[1] = (ip >> 16) & 0xFF;
    m_ip[2] = (ip >> 8) & 0xFF;
    m_ip[3] = ip & 0xFF;
}

void IPv4::setIp(uint8_t ip1, uint8_t ip2, uint8_t ip3, uint8_t ip4)
{
    m_ip[0] = ip1;
    m_ip[1] = ip2;
    m_ip[2] = ip3;
    m_ip[3] = ip4;
}

void IPv4::setIp(char *ip)
{
    sscanf(ip, "%hhu.%hhu.%hhu.%hhu", &m_ip[0], &m_ip[1], &m_ip[2], &m_ip[3]);
}

void IPv4::setIp(IPv4 &ip)
{
    m_ip[0] = ip.m_ip[0];
    m_ip[1] = ip.m_ip[1];
    m_ip[2] = ip.m_ip[2];
    m_ip[3] = ip.m_ip[3];
}

void IPv4::setIp(std::string &ip)
{
    setIp(ip);
}

bool IPv4::operator==(const IPv4 &ip)
{
    return m_ip[0] == ip.m_ip[0] && m_ip[1] == ip.m_ip[1] && m_ip[2] == ip.m_ip[2] && m_ip[3] == ip.m_ip[3];
}

bool IPv4::operator!=(const IPv4 &ip)
{
    return m_ip[0] != ip.m_ip[0] || m_ip[1] != ip.m_ip[1] || m_ip[2] != ip.m_ip[2] || m_ip[3] != ip.m_ip[3];
}

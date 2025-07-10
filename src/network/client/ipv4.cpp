#include "ipv4.hpp"
#include "tools/logStream/logStream.hpp"

Ipv4::Ipv4()
{
    m_ip[0] = 0;
    m_ip[1] = 0;
    m_ip[2] = 0;
    m_ip[3] = 0;
}

Ipv4::Ipv4(uint8_t ip1, uint8_t ip2, uint8_t ip3, uint8_t ip4)
{
    m_ip[0] = ip1;
    m_ip[1] = ip2;
    m_ip[2] = ip3;
    m_ip[3] = ip4;
}

Ipv4::Ipv4(uint32_t ip)
{
    m_ip[0] = (ip >> 24) & 0xFF;
    m_ip[1] = (ip >> 16) & 0xFF;
    m_ip[2] = (ip >> 8) & 0xFF;
    m_ip[3] = ip & 0xFF;
}

Ipv4::Ipv4(char *ip)
{
    setIp(ip);
}

Ipv4::Ipv4(const char *ip)
{
    setIp(ip);
}

Ipv4::~Ipv4()
{
}

uint8_t Ipv4::getIp1() const
{
    return m_ip[0];
}

uint8_t Ipv4::getIp2() const
{
    return m_ip[1];
}

uint8_t Ipv4::getIp3() const
{
    return m_ip[2];
}

uint8_t Ipv4::getIp4() const
{
    return m_ip[3];
}

uint32_t Ipv4::getIp() const
{
    return (m_ip[0] << 24) | (m_ip[1] << 16) | (m_ip[2] << 8) | m_ip[3];
}

std::string Ipv4::getIpString() const
{
    return std::to_string(m_ip[0]) + "." + std::to_string(m_ip[1]) + "." + std::to_string(m_ip[2]) + "." + std::to_string(m_ip[3]);
}

void Ipv4::setIp1(uint8_t ip1)
{
    m_ip[0] = ip1;
}

void Ipv4::setIp2(uint8_t ip2)
{
    m_ip[1] = ip2;
}

void Ipv4::setIp3(uint8_t ip3)
{
    m_ip[2] = ip3;
}

void Ipv4::setIp4(uint8_t ip4)
{
    m_ip[3] = ip4;
}

void Ipv4::setIp(uint32_t ip)
{
    m_ip[0] = (ip >> 24) & 0xFF;
    m_ip[1] = (ip >> 16) & 0xFF;
    m_ip[2] = (ip >> 8) & 0xFF;
    m_ip[3] = ip & 0xFF;
}

void Ipv4::setIp(uint8_t ip1, uint8_t ip2, uint8_t ip3, uint8_t ip4)
{
    m_ip[0] = ip1;
    m_ip[1] = ip2;
    m_ip[2] = ip3;
    m_ip[3] = ip4;
}

void Ipv4::setIp(char *ip)
{
    if (ip == nullptr)
    {
        LogStream::cout << "Error: IP address is null" << LogStream::endl;
        return;
    }
    unsigned int ip1, ip2, ip3, ip4;
    if (sscanf(ip, "%u.%u.%u.%u", &ip1, &ip2, &ip3, &ip4) != 4 ||
        ip1 > 255 || ip2 > 255 || ip3 > 255 || ip4 > 255)
    {
        LogStream::cout << "Error: Invalid IP address format: " << ip << LogStream::endl;
        return;
    }
    m_ip[0] = static_cast<uint8_t>(ip1);
    m_ip[1] = static_cast<uint8_t>(ip2);
    m_ip[2] = static_cast<uint8_t>(ip3);
    m_ip[3] = static_cast<uint8_t>(ip4);
}

void Ipv4::setIp(const char *ip)
{
    if (ip == nullptr)
    {
        LogStream::cout << "Error: IP address is null" << LogStream::endl;
        return;
    }
    unsigned int ip1, ip2, ip3, ip4;
    if (sscanf(ip, "%u.%u.%u.%u", &ip1, &ip2, &ip3, &ip4) != 4 ||
        ip1 > 255 || ip2 > 255 || ip3 > 255 || ip4 > 255)
    {
        LogStream::cout << "Error: Invalid IP address format: " << ip << LogStream::endl;
        return;
    }
    m_ip[0] = static_cast<uint8_t>(ip1);
    m_ip[1] = static_cast<uint8_t>(ip2);
    m_ip[2] = static_cast<uint8_t>(ip3);
    m_ip[3] = static_cast<uint8_t>(ip4);
}

void Ipv4::setIp(Ipv4 &ip)
{
    m_ip[0] = ip.m_ip[0];
    m_ip[1] = ip.m_ip[1];
    m_ip[2] = ip.m_ip[2];
    m_ip[3] = ip.m_ip[3];
}

void Ipv4::setIp(std::string &ip)
{
    setIp(ip);
}

bool Ipv4::operator==(const Ipv4 &ip) const
{
    return m_ip[0] == ip.m_ip[0] && m_ip[1] == ip.m_ip[1] && m_ip[2] == ip.m_ip[2] && m_ip[3] == ip.m_ip[3];
}

bool Ipv4::operator!=(const Ipv4 &ip) const
{
    return !(*this == ip);
}

#include "client.hpp"
#include "api/udp/udp.hpp"

Client::Client()
{
}

Client::Client(Ipv4 ip, MacAddr mac) : m_ip(ip), m_mac(mac)
{
}

Client::Client(const Ipv4& ip, const char* macAddress) : m_ip(ip), m_mac(macAddress)
{
}

Client::Client(const Client& other) : m_ip(other.m_ip), m_mac(other.m_mac)
{
}

Client& Client::operator=(const Client& other)
{
    if (this != &other)
    {
        m_ip = other.m_ip;
        m_mac = other.m_mac;
    }
    return *this;
}

const Ipv4& Client::getIP() const
{
    return m_ip;
}

const Ipv4& Client::getIp() const
{
    return m_ip;
}

std::string Client::getMacAddress() const
{
    return m_mac.getMacString();
}

const MacAddr& Client::getMAC() const
{
    return m_mac;
}

void Client::setIP(Ipv4 ip)
{
    m_ip = ip;
}

void Client::setMAC(MacAddr mac)
{
    m_mac = mac;
}

bool Client::operator==(const Client &client) const
{
    return m_mac == client.getMAC() && m_ip == client.getIP();
}

bool Client::operator!=(const Client &client) const
{
    return !(*this == client);
}

bool Client::operator<(const Client &client) const
{
    return false; // Dummy operator, do nothing
}

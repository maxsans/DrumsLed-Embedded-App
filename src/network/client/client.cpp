#include "client.h"
#include "api/udp/udp.h"

Client::Client()
{
}

Client::Client(Ipv4 ip, MacAddr mac)
{
    m_ip = ip;
    m_mac = mac;
}

const Ipv4& Client::getIP() const
{
    return m_ip;
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
    return m_mac == client.getMAC();
}

bool Client::operator!=(const Client &client) const
{
    return !(*this == client);
}

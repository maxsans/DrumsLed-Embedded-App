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

Client::Client(Client &client)
{
    m_ip = client.getIP();
    m_mac = client.getMAC();
}

Client::~Client()
{
}

Ipv4 Client::getIP()
{
    return m_ip;
}

MacAddr Client::getMAC()
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

void Client::send(char *data, uint32_t size)
{
    udp_send(data, size, m_ip.getIpString().c_str(), UDP_DEFAULT_PORT);
}

bool Client::operator==(Client &client)
{
    return (m_ip == client.getIP() && m_mac == client.getMAC());
}

bool Client::operator!=(Client &client)
{
    return (m_ip != client.getIP() || m_mac != client.getMAC());
}

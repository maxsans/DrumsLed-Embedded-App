#include "client.h"
#include "udp.h"

Client::Client()
{
}

Client::Client(IPv4 ip, MacAddr mac)
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

IPv4 Client::getIP()
{
    return m_ip;
}

MacAddr Client::getMAC()
{
    return m_mac;
}

void Client::setIP(IPv4 ip)
{
    m_ip = ip;
}

void Client::setMAC(MacAddr mac)
{
    m_mac = mac;
}

void Client::send(char *data, uint32_t size)
{
    g_udp.send(*this, data, size);
}

bool Client::operator==(Client &client)
{
    return (m_ip == client.getIP() && m_mac == client.getMAC());
}

bool Client::operator!=(Client &client)
{
    return (m_ip != client.getIP() || m_mac != client.getMAC());
}

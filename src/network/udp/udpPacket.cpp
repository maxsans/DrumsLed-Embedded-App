#include "udpPacket.h"

#include "string.h"

udpPacket::udpPacket()
{
    // Constructor
}

udpPacket::udpPacket(Client client, char *packet, uint32_t len)
{
    // Constructor
    m_client = client;
    memcpy(m_data, packet, len);
    m_len = len;
}


udpPacket::~udpPacket()
{
    // Destructor
}

void udpPacket::setPacket(Client client, char *packet, uint32_t len)
{
    // Set the packet
    m_client = client;
    memcpy(m_data, packet, len);
    m_len = len;
}

void udpPacket::setClient(Client client)
{
    // Set the client
    m_client = client;
}

void udpPacket::setData(char *packet, uint32_t len)
{
    // Set the data
    memcpy(m_data, packet, len);
    m_len = len;
}

void udpPacket::setLen(uint32_t len)
{
    // Set the length
    m_len = len;
}

char *udpPacket::getData()
{
    // Get the packet
    return m_data;
}

uint32_t udpPacket::getLen()
{
    // Get the length
    return m_len;
}

Client udpPacket::getClient()
{
    // Get the client
    return m_client;
}

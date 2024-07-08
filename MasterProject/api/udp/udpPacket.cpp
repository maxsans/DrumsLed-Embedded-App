#include "udpPacket.h"

#include "string.h"

udpPacket::udpPacket()
{
    // Constructor
}

udpPacket::~udpPacket()
{
    // Destructor
}

void udpPacket::setPacket(char *packet, char *ip)
{
    // Set the packet
    memccpy(m_packet, packet, 0, BUFLEN);
    strcpy(m_ip, ip);
}

void udpPacket::setIp(char *ip)
{
    // Set the ip
    strcpy(m_ip, ip);
}

char *udpPacket::getPacket()
{
    // Get the packet
    return m_packet;
}

char *udpPacket::getIp()
{
    // Get the ip
    return m_ip;
}

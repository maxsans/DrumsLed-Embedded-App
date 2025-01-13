#include "udpPacket.h"

UdpPacket::UdpPacket(UdpPacketType type) : m_type(type)
{
}

UdpPacket::UdpPacket(Client client, UdpPacketType type) : m_type(type)
{
    m_client = client;
}


#include "udpPacket.h"

UdpPacket::UdpPacket(Client client, UdpPacketType type) : m_type(type)
{
    m_client = client;
}

UdpPacket::~UdpPacket()
{

}

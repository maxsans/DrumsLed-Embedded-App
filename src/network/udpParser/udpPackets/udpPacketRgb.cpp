#include "udpPacketRgb.h"

UdpPacketRgb::UdpPacketRgb(Client client, char *data) : UdpPacket(client, PACKET_TYPE_RGB)
{
    m_color.setColor(data[0], data[1], data[2]);
}

UdpPacketRgb::UdpPacketRgb(Client client, rgbColor color) : UdpPacket(client, PACKET_TYPE_RGB)
{
    m_color = color;
}

void UdpPacketRgb::parse()
{
    // Implement parsing logic
}

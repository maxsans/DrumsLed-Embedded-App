#include "udpPacketRgb.h"

#include "api/udp/udp.h"
#ifdef __TARGET_SLAVES

#endif

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
#ifdef __TARGET_SLAVES

#endif
}

void UdpPacketRgb::send()
{
    char l_packet[] =
    {
        PACKET_TYPE_RGB,
        (char)m_color.getRed(),
        (char)m_color.getGreen(),
        (char)m_color.getBlue()
    };
    udp_send(l_packet, sizeof(l_packet), m_client.getIP().getIpString().c_str(), UDP_DEFAULT_PORT);
}

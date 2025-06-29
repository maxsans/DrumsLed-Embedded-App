#include "udpPacketRgb.h"
#include "tools/logStream/logStream.h"

#include "api/udp/udp.h"
#ifdef __TARGET_SLAVES
#include "slaves/kit/kit.h"
#endif

UdpPacketRgb::UdpPacketRgb(Client client, char *data) : UdpPacket(client, PACKET_TYPE_RGB)
{
    m_color.setColor(data[0], data[1], data[2]);
}

UdpPacketRgb::UdpPacketRgb(Client client, RgbColor color) : UdpPacket(client, PACKET_TYPE_RGB)
{
    m_color = color;
}

void UdpPacketRgb::parse()
{
#ifdef __TARGET_SLAVES
    // Set the color in the Kit
    uint8_t red = m_color.getRed();
    uint8_t green = m_color.getGreen();
    uint8_t blue = m_color.getBlue();
    // Log the received RGB color
    LogStream::cout << "Received RGB color: " << red << ", " << green << ", " << blue << LogStream::endl;
    Kit::getLeds()->fill(red, green, blue);
    Kit::getLeds()->show();
#endif
}

void UdpPacketRgb::send()
{
    char l_packet[] =
        {
            PACKET_TYPE_RGB,
            (char)m_color.getRed(),
            (char)m_color.getGreen(),
            (char)m_color.getBlue()};
//     udp_send(l_packet, sizeof(l_packet), m_client.getIP().getIpString().c_str(), UDP_DEFAULT_PORT);
}

#include "udpPacketAdc.h"

#include "api/udp/udp.h"

UdpPacketAdc::UdpPacketAdc(Client client, char *data) : UdpPacket(client, PACKET_TYPE_ADC)
{
    m_adcValue = data[0];
}

UdpPacketAdc::UdpPacketAdc(Client client, uint8_t adcValue) : UdpPacket(client, PACKET_TYPE_ADC)
{
    m_adcValue = adcValue;
}

void UdpPacketAdc::parse()
{
    // Implement parsing logic
}

void UdpPacketAdc::send()
{
    char l_packet[] =
    {
        PACKET_TYPE_ADC,
        (char)m_adcValue
    };
    udp_send(l_packet, sizeof(l_packet), m_client.getIP().getIpString().c_str(), UDP_DEFAULT_PORT);
}

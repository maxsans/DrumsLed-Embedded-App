#include "udpPacketAdc.h"

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

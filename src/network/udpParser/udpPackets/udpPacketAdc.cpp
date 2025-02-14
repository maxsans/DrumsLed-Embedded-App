#include "udpPacketAdc.h"

#include "api/udp/udp.h"
#ifdef __TARGET_MASTER
    #include "session/session.h"
#endif

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
#ifdef __TARGET_MASTER
    // New value on the adc of this module, uptate it micro value
    Module *l_module = g_session.getModuleManager()->getModule(m_client);
    if (l_module != NULL)
    {
        // Set the micro value of this module
        g_session.getModuleManager()->setMicro(m_client, m_adcValue);
    }
#endif
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

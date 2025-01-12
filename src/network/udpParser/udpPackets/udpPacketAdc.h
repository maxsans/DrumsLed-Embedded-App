#ifndef __UDP_PACKET_ADC_H__
#define __UDP_PACKET_ADC_H__

#include "udpPacket.h"

class UdpPacketAdc : public UdpPacket
{
    private:
        uint8_t m_adcValue;

    public:
        UdpPacketAdc(Client client, char *data);
        UdpPacketAdc(Client client, uint8_t adcValue);
        void parse() override;
        void send() override;
};

#endif // __UDP_PACKET_ADC_H__

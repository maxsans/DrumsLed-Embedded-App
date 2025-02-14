#ifndef __UDP_PACKET_RGB_H__
#define __UDP_PACKET_RGB_H__

#include "udpPacket.h"
#include "led/rgbColor.h"

class UdpPacketRgb : public UdpPacket
{
    private:
        RgbColor m_color;

    public:
        UdpPacketRgb(Client client, char *data);
        UdpPacketRgb(Client client, RgbColor color);
        virtual ~UdpPacketRgb() = default;
        void parse() override;
        void send() override;
};

#endif // __UDP_PACKET_RGB_H__

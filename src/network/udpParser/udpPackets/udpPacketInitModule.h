#ifndef __UDP_PACKET_INIT_H__
#define __UDP_PACKET_INIT_H__

#include "udpPacket.h"

class UdpPacketInitModule : public UdpPacket
{
    public:
        UdpPacketInitModule(Client client, char *data);
        void parse() override;
        void send() override;
};

#endif // __UDP_PACKET_INIT_H__

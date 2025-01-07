#ifndef __UDP_PACKET_INIT_H__
#define __UDP_PACKET_INIT_H__

#include "udpPacket.h"

class UdpPacketInit : public UdpPacket
{
    public:
        UdpPacketInit(Client client, char *data);
        void parse() override;
};

#endif // __UDP_PACKET_INIT_H__

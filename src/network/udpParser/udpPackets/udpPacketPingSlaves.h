#ifndef __UDP_PACKET_PING_SLAVES_H__
#define __UDP_PACKET_PING_SLAVES_H__

#include "udpPacket.h"

class UdpPacketPingSlaves : public UdpPacket
{
    public:
        UdpPacketPingSlaves();
        UdpPacketPingSlaves(Client client);
        void parse() override;
        void send() override;
};

#endif // __UDP_PACKET_PING_SLAVES_H__

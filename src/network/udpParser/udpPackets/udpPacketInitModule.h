#ifndef __UDP_PACKET_INIT_H__
#define __UDP_PACKET_INIT_H__

#include "udpPacket.h"

typedef enum
{
    TYPE_DRUM_MODULE
} moduleType_t;

class UdpPacketInitModule : public UdpPacket
{
    private:
        moduleType_t m_moduleType;

    public:
        UdpPacketInitModule(Client client, char *data);
        UdpPacketInitModule(Client client, moduleType_t moduleType);
        void parse() override;
        void send() override;
};

#endif // __UDP_PACKET_INIT_H__

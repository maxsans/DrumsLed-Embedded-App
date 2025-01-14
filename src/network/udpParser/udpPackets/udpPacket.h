#ifndef __UDP_PACKET_H__
#define __UDP_PACKET_H__

#include "network/client/client.h"

typedef enum
{
    PACKET_TYPE_PING_SLAVES,
    PACKET_TYPE_INIT_MODULE,
    PACKET_TYPE_RGB,
    PACKET_TYPE_ADC
    // Add new packet types here
} UdpPacketType;

class UdpPacket
{
    private:
        const UdpPacketType m_type;

    protected:
        Client m_client;

    public:
        UdpPacket(UdpPacketType type);
        UdpPacket(Client client, UdpPacketType type);

        /**
         * @brief Virtual method to do the action of the packet
         */
        virtual void parse() = 0;

        virtual void send() = 0;

        virtual ~UdpPacket() = default;
};

#endif // __UDP_PACKET_H__

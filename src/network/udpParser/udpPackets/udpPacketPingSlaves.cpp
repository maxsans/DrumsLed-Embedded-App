#include "udpPacketPingSlaves.h"

#include "api/logs/logStream.h"
#include "api/udp/udp.h"

UdpPacketPingSlaves::UdpPacketPingSlaves() : UdpPacket(Client(), PACKET_TYPE_PING_SLAVES)
{
    // No this packet type does not have any data
}

void UdpPacketPingSlaves::parse()
{
    // Implement parsing logic
    LogStream() << "Ping slaves packet received. Master : " << m_client.getIP().getIpString() << LogStream::endl;
}

void UdpPacketPingSlaves::send()
{
    char l_packet[] =
    {
        PACKET_TYPE_PING_SLAVES
    };
    udp_send_broadcast(l_packet, sizeof(l_packet), UDP_DEFAULT_PORT);
}

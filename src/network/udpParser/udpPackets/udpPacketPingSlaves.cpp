#include "udpPacketPingSlaves.h"

#include "tools/logStream/logStream.h"
#include "api/udp/udp.h"

UdpPacketPingSlaves::UdpPacketPingSlaves() : UdpPacket(PACKET_TYPE_PING_SLAVES)
{
}

void UdpPacketPingSlaves::parse()
{
    // Implement parsing logic
    LogStream::cout << "Ping slaves packet received. Master : " << m_client.getIP().getIpString() << LogStream::endl;
}

void UdpPacketPingSlaves::send()
{
    char l_packet[] =
    {
        PACKET_TYPE_PING_SLAVES
    };
    udp_send_broadcast(l_packet, sizeof(l_packet), UDP_DEFAULT_PORT);
}

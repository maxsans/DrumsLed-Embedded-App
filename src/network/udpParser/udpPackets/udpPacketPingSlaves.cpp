#include "udpPacketPingSlaves.h"

#include "tools/logStream/logStream.h"
#include "api/udp/udp.h"
#include "slaves/kit/kit.h"

UdpPacketPingSlaves::UdpPacketPingSlaves() : UdpPacket(PACKET_TYPE_PING_SLAVES)
{
}

UdpPacketPingSlaves::UdpPacketPingSlaves(Client client) : UdpPacket(client, PACKET_TYPE_PING_SLAVES)
{
}

void UdpPacketPingSlaves::parse()
{
#ifdef __TARGET_SLAVES
    // Set the master client in the Kit
    Kit::setMasterClient(m_client);
#endif
}

void UdpPacketPingSlaves::send()
{
    char l_packet[] =
        {
            PACKET_TYPE_PING_SLAVES};
    udp_send_broadcast(l_packet, sizeof(l_packet), UDP_DEFAULT_PORT);
}

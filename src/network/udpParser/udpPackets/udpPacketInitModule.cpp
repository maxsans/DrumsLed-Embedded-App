#include "udpPacketInitModule.h"

#include "api/logs/logStream.h"
#include "api/udp/udp.h"

UdpPacketInitModule::UdpPacketInitModule(Client client, char *data) : UdpPacket(client, PACKET_TYPE_INIT_MODULE)
{
    // No this packet type does not have any data
}

void UdpPacketInitModule::parse()
{
    // Implement parsing logic
    LogStream() << "Init packet received" << LogStream::endl;
}

void UdpPacketInitModule::send()
{
    char l_packet[] =
    {
        PACKET_TYPE_INIT_MODULE
    };
    udp_send(l_packet, sizeof(l_packet), m_client.getIP().getIpString().c_str(), UDP_DEFAULT_PORT);
}

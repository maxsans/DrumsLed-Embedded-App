#include "udpPacketInit.h"

#include "api/logs/logStream.h"

UdpPacketInit::UdpPacketInit(Client client, char *data) : UdpPacket(client, PACKET_TYPE_INIT)
{
    // No this packet type does not have any data
}

void UdpPacketInit::parse()
{
    // Implement parsing logic
    LogStream() << "Init packet received" << LogStream::endl;
}

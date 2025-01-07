#include "udpPacketInit.h"

#include <iostream>

UdpPacketInit::UdpPacketInit(Client client, char *data) : UdpPacket(client, PACKET_TYPE_INIT)
{
    // No this packet type does not have any data
}

void UdpPacketInit::parse()
{
    // Implement parsing logic
    std::cout << "Init packet received" << std::endl;
}

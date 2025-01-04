#include "getUdpPacket.h"
#include "api/udp/udp.h"
#include "udpPacket.h"

#define MAX_UDP_PACKET_SIZE 1024
#define MAX_IP_SIZE_CHARS 16

bool getUdpPacket(udpPacket *packet)
{
    char l_PacketData[MAX_UDP_PACKET_SIZE];
    char l_IP[MAX_IP_SIZE_CHARS];
    char l_Mac[18];
    int16_t l_Port;
    uint32_t l_len = udp_recv(l_PacketData, MAX_UDP_PACKET_SIZE, l_IP, &l_Port, l_Mac);
    if (l_len)
    {
        // Get the IP address of the packet
        IPv4 l_packetIp(l_IP);
        // Get the MAC address of the packet
        MacAddr l_packetMac(l_Mac);
        // Get the client from the IP address and MAC address
        Client l_packetClient(l_packetIp, l_packetMac);
        // Set the packet
        packet->setPacket(l_packetClient, l_PacketData, l_len);
    }
    // Return true if a packet was received
    return (l_len > 0);
}

#include "udpParser.h"

#include "api/udp/udp.h"

#include "udpPackets/udpPacketInit.h"
#include "udpPackets/udpPacketRgb.h"
#include "udpPackets/udpPacketAdc.h"

#define MAX_UDP_PACKET_SIZE 1024
#define MAX_IP_SIZE_CHARS 16

UdpPacket *UdpParser::identify(char *data, Client client)
{
    UdpPacketType type = (UdpPacketType)data[0];
    UdpPacket *packet = nullptr;
    char *dataWithoutType = data + sizeof(UdpPacketType);

    switch (type)
    {
#ifdef __TARGET_MASTER // If master
        case PACKET_TYPE_INIT:
            packet = new UdpPacketInit(client, dataWithoutType);
            break;
        case PACKET_TYPE_ADC:
            packet = new UdpPacketAdc(client, dataWithoutType);
            break;
        // Add new packet types here
#endif

#ifdef __TARGET_SLAVES // If slave
        case PACKET_TYPE_RGB:
            packet = new UdpPacketRgb(client, dataWithoutType);
            break;
        // Add new packet types here
#endif

        default:
            break;
    }

    return packet;
}

void UdpParser::process()
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
        // Parse the packet
        UdpParser::parseUdp(l_PacketData, l_packetClient);
    }
}

void UdpParser::parseUdp(char *data, Client client)
{
    UdpPacket *packet = identify(data, client);
    if(packet != nullptr)
    {
        packet->parse();
        delete packet;
    }
}

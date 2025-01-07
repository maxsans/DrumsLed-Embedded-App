#ifndef __UDP_PARSER_H__
#define __UDP_PARSER_H__

#include "tools/timeTools/periodicCallsMs.h"
#include "udpPackets/udpPacket.h"
#include "network/client/client.h"

/**
 * @brief Class to parse UDP packets.
 */
class UdpParser
{
    private:
        /**
         * @brief This method identifies the packet and return an instance of it
         * @param data Received data
         * @param client Client that sent the packet
         * @return UdpPacket* Packet identified
         */
        static UdpPacket *identify(char *data, Client client);

    public:
        /**
         * @brief Process the UDP packets
         * @note This function must be called in the main loop.
         */
        static void process();
        /**
         * @brief Parse a UDP packet
         * @param data Received data
         */
        static void parseUdp(char *data);
        /**
         * @brief Parse a UDP packet
         * @param data Received data
         * @param client Client that sent the packet
         */
        static void parseUdp(char *data, Client client);
};

#endif

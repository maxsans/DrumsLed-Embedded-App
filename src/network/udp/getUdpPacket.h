#ifndef __GET_UDP_PACKET_H__
#define __GET_UDP_PACKET_H__

#include "udpPacket.h"

/**
 * @brief Get a UDP packet if available
 *
 * @param [out] packet Packet to be filled
 * @return true if a packet was received
 * @return false if no packet was received
 */
bool getUdpPacket(udpPacket *packet);

#endif // __GET_UDP_PACKET_H__

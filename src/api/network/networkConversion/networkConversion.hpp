/**
 * @file networkConversion.hpp
 * @brief Header file for network conversion utilities
 * @author Cyprien Ménard
 * @date 2025-08-22
 */

#ifndef __NETWORKCONVERSION_HPP__
#define __NETWORKCONVERSION_HPP__

#include <stdint.h>

/**
 * @brief Convert 32-bit value from host to network byte order
 * @param hostlong 32-bit value in host byte order
 * @return 32-bit value in network byte order
 */
uint32_t network_htonl(uint32_t hostlong);

/**
 * @brief Convert 16-bit value from host to network byte order
 * @param hostshort 16-bit value in host byte order
 * @return 16-bit value in network byte order
 */
uint16_t network_htons(uint16_t hostshort);

/**
 * @brief Convert 32-bit value from network to host byte order
 * @param netlong 32-bit value in network byte order
 * @return 32-bit value in host byte order
 */
uint32_t network_ntohl(uint32_t netlong);

/**
 * @brief Convert 16-bit value from network to host byte order
 * @param netshort 16-bit value in network byte order
 * @return 16-bit value in host byte order
 */
uint16_t network_ntohs(uint16_t netshort);

/**
 * @brief Convert 64-bit value from host to network byte order
 * @param hostlonglong 64-bit value in host byte order
 * @return 64-bit value in network byte order
 */
uint64_t network_htonll(uint64_t hostlonglong);

/**
 * @brief Convert 64-bit value from network to host byte order
 * @param netlonglong 64-bit value in network byte order
 * @return 64-bit value in host byte order
 */
uint64_t network_ntohll(uint64_t netlonglong);

/**
 * @brief Convert IP address string to network byte order
 * @param cp IP address string (e.g., "192.168.1.1")
 * @return IP address in network byte order, or 0xFFFFFFFF on error
 */
uint32_t network_inet_addr(const char *cp);

/**
 * @brief Convert IP address from network byte order to string
 * @param addr IP address in network byte order
 * @return Static string containing IP address (not thread-safe)
 */
const char *network_inet_ntoa(uint32_t addr);

#endif // __NETWORKCONVERSION_HPP__

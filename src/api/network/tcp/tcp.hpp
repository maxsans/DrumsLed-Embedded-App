#ifndef __TCP_HPP__
#define __TCP_HPP__

#include "network/interCom/interComParser/incompletMsg/incompletMsg.hpp"
#include <stdint.h>
#include <vector>

#define TCP_DEFAULT_PORT 8888

/**
 * @brief Function to initialize the TCP connection
 */
void tcp_init();

/**
 * @brief Function to send data over TCP
 * @param data Data to be sent
 * @param len Length of the data
 * @param ip IP address of the receiver
 * @param port Port number of the receiver
 */
void tcp_send(const char *data, int16_t len, const char *ip, int16_t port);

/**
 * @brief Function to receive data over TCP
 * @return Vector of incomplete messages
 */
std::vector<IncompletMsg> tcp_recv();

/**
 * @brief Function to get the IP address of the host
 * @param ip Buffer to store the IP address
 */
void tcp_get_host_ip(char *ip);

/**
 * @brief Function to get the MAC address of the host
 * @param mac Buffer to store the MAC address
 */
void tcp_get_host_mac(char *mac);

#endif // __TCP_HPP__

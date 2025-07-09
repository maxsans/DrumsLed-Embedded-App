#ifndef __UDP_HPP__
#define __UDP_HPP__

#include <stdint.h>

#define UDP_DEFAULT_PORT 9999

/**
 * @brief Function to initialize the UDP connection
 */
void udp_init();

/**
 * @brief Function to send data over UDP
 *
 * @param data Data to be sent
 * @param len Length of the data
 * @param ip IP address of the receiver
 * @param port Port number of the receiver
 */
void udp_send(const char *data, int16_t len, const char *ip, int16_t port);

/**
 * @brief Function to send data over UDP to all devices in the network
 *
 * @param data Data to be sent
 * @param len Length of the data
 * @param port Port number of the receiver
 */
void udp_send_broadcast(const char *data, int16_t len, int16_t port);

/**
 * @brief Function to receive data over UDP
 *
 * @param data Buffer to store the received data
 * @param len Length of the buffer
 * @param ip IP address of the sender
 * @param port Port number of the sender
 * @param mac MAC address of the sender
 *
 * @return Number of bytes received
 */
uint32_t udp_recv(char *data, int16_t len, char *ip, int16_t *port, char *mac);

/**
 * @brief Function to get the IP address of the host
 *
 * @param ip Buffer to store the IP address
 */
void udp_get_host_ip(char *ip);

/**
 * @brief Function to get the MAC address of the host
 *
 * @param mac Buffer to store the MAC address
 */
void udp_get_host_mac(char *mac);

#endif // __UDP_HPP__

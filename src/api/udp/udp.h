#ifndef __UDP_H__
#define __UDP_H__

#include <stdint.h>

#define UDP_DEFAULT_PORT 4321

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
 */
void udp_recv(char *data, int16_t len, char *ip, int16_t *port);

#endif // __UDP_H__

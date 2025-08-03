#ifndef __TCP_HPP__
#define __TCP_HPP__

#include <stdint.h>
#include <functional>

#define TCP_DEFAULT_PORT 8888

/**
 * @brief Callback to call when a tcp packet is received
 */
using tcp_recv_callback_t = std::function<void(const char *data, int16_t len, const char *ip, int16_t port, const char *mac)>;

/**
 * @brief Function to initialize the TCP connection
 * @param tcp_recv_callback Callback to call when a tcp packet is received
 */
void tcp_init(tcp_recv_callback_t tcp_recv_callback);

/**
 * @brief Function to send data over TCP
 *
 * @param data Data to be sent
 * @param len Length of the data
 * @param ip IP address of the receiver
 * @param port Port number of the receiver
 */
void tcp_send(const char *data, int16_t len, const char *ip, int16_t port);

/**
 * @brief Function to get the IP address of the host
 *
 * @param ip Buffer to store the IP address
 */
void tcp_get_host_ip(char *ip);

/**
 * @brief Function to get the MAC address of the host
 *
 * @param mac Buffer to store the MAC address
 */
void tcp_get_host_mac(char *mac);

#endif // __TCP_HPP__

#include "udp.h"

#include <lwip/sockets.h>
#include <lwip/netdb.h>
#include <string.h>
#include <esp_wifi.h>

int udp_socket;
struct sockaddr_in udp_addr;

void udp_init()
{
    udp_socket = socket(AF_INET, SOCK_DGRAM, 0);
    if (udp_socket < 0) {
        // Handle error
    }
    memset(&udp_addr, 0, sizeof(udp_addr));
    udp_addr.sin_family = AF_INET;
}

void udp_send(const char *data, int16_t len, const char *ip, int16_t port)
{
    udp_addr.sin_port = htons(port);
    inet_pton(AF_INET, ip, &udp_addr.sin_addr.s_addr);
    sendto(udp_socket, data, len, 0, (struct sockaddr *)&udp_addr, sizeof(udp_addr));
}

void udp_send_broadcast(const char *data, int16_t len, int16_t port)
{
    int broadcastEnable = 1;
    setsockopt(udp_socket, SOL_SOCKET, SO_BROADCAST, &broadcastEnable, sizeof(broadcastEnable));
    udp_addr.sin_port = htons(port);
    udp_addr.sin_addr.s_addr = htonl(INADDR_BROADCAST);
    sendto(udp_socket, data, len, 0, (struct sockaddr *)&udp_addr, sizeof(udp_addr));
}

uint32_t udp_recv(char *data, int16_t len, char *ip, int16_t *port, char *mac)
{
    struct sockaddr_in src_addr;
    socklen_t addr_len = sizeof(src_addr);
    int recv_len = recvfrom(udp_socket, data, len, 0, (struct sockaddr *)&src_addr, &addr_len);
    if (recv_len > 0) {
        inet_ntoa_r(src_addr.sin_addr, ip, INET_ADDRSTRLEN);
        *port = ntohs(src_addr.sin_port);

        // Retrieve MAC address from ARP table
        struct eth_addr *eth_ret;
        ip4_addr_t ipaddr;
        inet_pton(AF_INET, ip, &ipaddr);
        eth_ret = etharp_find_addr(netif_default, &ipaddr, (struct eth_addr *)mac, NULL);
        if (eth_ret != NULL) {
            sprintf(mac, "%02x:%02x:%02x:%02x:%02x:%02x",
                    eth_ret->addr[0], eth_ret->addr[1], eth_ret->addr[2],
                    eth_ret->addr[3], eth_ret->addr[4], eth_ret->addr[5]);
        }
    }
    return recv_len;
}

void udp_get_host_ip(char *ip)
{
    struct ip_addr ipaddr;
    ipaddr.addr = netif_default->ip_addr.addr;
    inet_ntoa_r(ipaddr, ip, INET_ADDRSTRLEN);
}

void udp_get_host_mac(char *mac)
{
    uint8_t mac_addr[6];
    esp_wifi_get_mac(ESP_IF_WIFI_STA, mac_addr);
    sprintf(mac, "%02x:%02x:%02x:%02x:%02x:%02x",
            mac_addr[0], mac_addr[1], mac_addr[2],
            mac_addr[3], mac_addr[4], mac_addr[5]);
}

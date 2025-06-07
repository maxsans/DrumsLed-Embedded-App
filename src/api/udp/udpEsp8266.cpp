#include "udp.h"

#include <lwip/sockets.h>
#include <lwip/netdb.h>
#include <lwip/etharp.h>
#include <string.h>
#include <esp_wifi.h>

int udp_socket;
struct sockaddr_in udp_addr;

void udp_init()
{
    udp_socket = socket(AF_INET, SOCK_DGRAM, 0);
    if (udp_socket < 0)
    {
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
    struct sockaddr_in source_addr;
    socklen_t socklen = sizeof(source_addr);
    int ret = recvfrom(udp_socket, data, len, 0, (struct sockaddr *)&source_addr, &socklen);
    if (ret > 0)
    {
        inet_ntop(AF_INET, &source_addr.sin_addr, ip, INET_ADDRSTRLEN);
        *port = ntohs(source_addr.sin_port);

        // Find MAC address using ARP table
        for (int i = 0; i < ARP_TABLE_SIZE; i++)
        {
            ip4_addr_t *ret_ip;
            struct netif *ret_netif;
            struct eth_addr *ret_eth;
            if (etharp_get_entry(i, &ret_ip, &ret_netif, &ret_eth) == 1)
            {
                sprintf(mac, "%02x:%02x:%02x:%02x:%02x:%02x",
                        ret_eth->addr[0], ret_eth->addr[1], ret_eth->addr[2],
                        ret_eth->addr[3], ret_eth->addr[4], ret_eth->addr[5]);
            }
        }

        printf("Received %d bytes from %s:%d, MAC: %s\n", ret, ip, *port, mac);
    }
    return ret;
}

void udp_get_host_ip(char *ip)
{
    tcpip_adapter_ip_info_t ip_info;
    tcpip_adapter_get_ip_info(TCPIP_ADAPTER_IF_STA, &ip_info);
    sprintf(ip, IPSTR, IP2STR(&ip_info.ip));
}

void udp_get_host_mac(char *mac)
{
    uint8_t mac_addr[6];
    esp_wifi_get_mac(ESP_IF_WIFI_STA, mac_addr);
    sprintf(mac, "%02x:%02x:%02x:%02x:%02x:%02x",
            mac_addr[0], mac_addr[1], mac_addr[2],
            mac_addr[3], mac_addr[4], mac_addr[5]);
}

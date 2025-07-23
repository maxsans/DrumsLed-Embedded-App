#include "udp.hpp"
#include <string.h>
#include <lwip/sockets.h>
#include <lwip/inet.h>
#include <lwip/netdb.h>
#include <esp_wifi.h>
#include <esp_system.h>

static int udp_sock = -1;

void udp_init()
{
    // Create a UDP socket if not already created
    if (udp_sock < 0)
    {
        udp_sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
        if (udp_sock >= 0)
        {
            struct sockaddr_in local_addr;
            memset(&local_addr, 0, sizeof(local_addr));
            local_addr.sin_family = AF_INET;
            local_addr.sin_addr.s_addr = htonl(INADDR_ANY);
            local_addr.sin_port = htons(UDP_DEFAULT_PORT);
            if (bind(udp_sock, (struct sockaddr *)&local_addr, sizeof(local_addr)) < 0)
            {
                printf("UDP bind failed on port %d\n", UDP_DEFAULT_PORT);
                close(udp_sock);
                udp_sock = -1;
            }
            else
            {
                printf("UDP socket bound to port %d\n", UDP_DEFAULT_PORT);
            }
        }
        else
        {
            printf("UDP socket creation failed\n");
        }
    }
}

void udp_send(const char *data, int16_t len, const char *ip, int16_t port)
{
    if (udp_sock < 0)
        udp_init();
    struct sockaddr_in dest_addr;
    memset(&dest_addr, 0, sizeof(dest_addr));
    dest_addr.sin_family = AF_INET;
    dest_addr.sin_port = htons(port);
    dest_addr.sin_addr.s_addr = inet_addr(ip);
    sendto(udp_sock, data, len, 0, (struct sockaddr *)&dest_addr, sizeof(dest_addr));
}

void udp_send_broadcast(const char *data, int16_t len, int16_t port)
{
    if (udp_sock < 0)
        udp_init();
    int broadcastEnable = 1;
    setsockopt(udp_sock, SOL_SOCKET, SO_BROADCAST, &broadcastEnable, sizeof(broadcastEnable));
    struct sockaddr_in dest_addr;
    memset(&dest_addr, 0, sizeof(dest_addr));
    dest_addr.sin_family = AF_INET;
    dest_addr.sin_port = htons(port);
    dest_addr.sin_addr.s_addr = htonl(INADDR_BROADCAST);
    sendto(udp_sock, data, len, 0, (struct sockaddr *)&dest_addr, sizeof(dest_addr));
}

uint32_t udp_recv(char *data, int16_t len, char *ip, int16_t *port, char *mac)
{
    if (udp_sock < 0)
    {
        printf("udp_recv: socket not initialized, calling udp_init()\n");
        udp_init();
    }
    if (udp_sock < 0)
    {
        printf("ERROR : UDP socket not initialized\n");
        return 0; // Socket not available
    }

    struct sockaddr_in src_addr;
    socklen_t addr_len = sizeof(src_addr);

    // Set non-blocking mode using fcntl (correct for ESP-IDF)
    int flags = fcntl(udp_sock, F_GETFL, 0);
    if (!(flags & O_NONBLOCK))
    {
        fcntl(udp_sock, F_SETFL, flags | O_NONBLOCK);
    }

    int recv_len = recvfrom(udp_sock, data, len, 0, (struct sockaddr *)&src_addr, &addr_len);
    if (recv_len > 0)
    {
        // printf("Received %d bytes from %s:%d\n", recv_len, inet_ntoa(src_addr.sin_addr), ntohs(src_addr.sin_port));
        if (ip)
        {
            inet_ntop(AF_INET, &src_addr.sin_addr, ip, INET_ADDRSTRLEN);
        }
        else
        {
            printf("IP address buffer is NULL\n");
        }
        if (port)
        {
            *port = ntohs(src_addr.sin_port);
        }
        else
        {
            printf("Port buffer is NULL\n");
        }
        // The MAC address is not needed on esp8266 module
        if (mac)
        {
            // So set it to a default value
            strcpy(mac, "00:00:00:00:00:00");
        }
        return recv_len;
    }
    else if (recv_len < 0)
    {
        int error = errno;
        if (error == EAGAIN || error == EWOULDBLOCK)
        {
            // No data received
            // printf("No UDP data available (EAGAIN/EWOULDBLOCK)\n");
            return 0;
        }
        else
        {
            printf("recvfrom() failed with error code : %d\n", error);
            return 0;
        }
    }
    // printf("udp_recv: recvfrom returned 0\n");
    return 0;
}

void udp_get_host_ip(char *ip)
{
    tcpip_adapter_ip_info_t ip_info;
    tcpip_adapter_get_ip_info(TCPIP_ADAPTER_IF_STA, &ip_info);
    strcpy(ip, ip4addr_ntoa(&ip_info.ip));
}

void udp_get_host_mac(char *mac)
{
    uint8_t mac_addr[6];
    esp_wifi_get_mac(WIFI_IF_STA, mac_addr);
    sprintf(mac, "%02X:%02X:%02X:%02X:%02X:%02X",
            mac_addr[0], mac_addr[1], mac_addr[2],
            mac_addr[3], mac_addr[4], mac_addr[5]);
}

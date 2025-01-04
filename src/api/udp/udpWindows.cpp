#include "udp.h"

#include <winsock2.h>
#include <ws2tcpip.h>

#pragma comment(lib, "ws2_32.lib")

SOCKET udp_socket;
struct sockaddr_in udp_addr;

void udp_init()
{
    WSADATA wsaData;
    WSAStartup(MAKEWORD(2, 2), &wsaData);
    udp_socket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if (udp_socket == INVALID_SOCKET) {
        // Handle error
    }
    memset(&udp_addr, 0, sizeof(udp_addr));
    udp_addr.sin_family = AF_INET;
}

void udp_send(const char *data, int16_t len, const char *ip, int16_t port)
{
    udp_addr.sin_port = htons(port);
    inet_pton(AF_INET, ip, &udp_addr.sin_addr);
    sendto(udp_socket, data, len, 0, (struct sockaddr *)&udp_addr, sizeof(udp_addr));
}

void udp_send_broadcast(const char *data, int16_t len, int16_t port)
{
    BOOL broadcastEnable = TRUE;
    setsockopt(udp_socket, SOL_SOCKET, SO_BROADCAST, (char *)&broadcastEnable, sizeof(broadcastEnable));
    udp_addr.sin_port = htons(port);
    udp_addr.sin_addr.s_addr = htonl(INADDR_BROADCAST);
    sendto(udp_socket, data, len, 0, (struct sockaddr *)&udp_addr, sizeof(udp_addr));
}

void udp_recv(char *data, int16_t len, char *ip, int16_t *port)
{
    struct sockaddr_in src_addr;
    int addr_len = sizeof(src_addr);
    int recv_len = recvfrom(udp_socket, data, len, 0, (struct sockaddr *)&src_addr, &addr_len);
    if (recv_len > 0) {
        inet_ntop(AF_INET, &src_addr.sin_addr, ip, INET_ADDRSTRLEN);
        *port = ntohs(src_addr.sin_port);
    }
}

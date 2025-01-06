#include "udp.h"

#include <winsock2.h>
#include <ws2tcpip.h>
#include <iphlpapi.h>

#pragma comment(lib, "ws2_32.lib")
#pragma comment(lib, "iphlpapi.lib")

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

uint32_t udp_recv(char *data, int16_t len, char *ip, int16_t *port, char *mac)
{
    struct sockaddr_in src_addr;
    int addr_len = sizeof(src_addr);
    int recv_len = recvfrom(udp_socket, data, len, 0, (struct sockaddr *)&src_addr, &addr_len);
    if (recv_len > 0) {
        inet_ntop(AF_INET, &src_addr.sin_addr, ip, INET_ADDRSTRLEN);
        *port = ntohs(src_addr.sin_port);

        // Retrieve MAC address from ARP table
        MIB_IPNETTABLE *arpTable = (MIB_IPNETTABLE *)malloc(sizeof(MIB_IPNETTABLE));
        ULONG size = 0;
        GetIpNetTable(arpTable, &size, 0);
        arpTable = (MIB_IPNETTABLE *)malloc(size);
        if (GetIpNetTable(arpTable, &size, 0) == NO_ERROR) {
            for (int i = 0; i < (int)arpTable->dwNumEntries; i++) {
                if (arpTable->table[i].dwAddr == src_addr.sin_addr.s_addr) {
                    sprintf(mac, "%02x:%02x:%02x:%02x:%02x:%02x",
                            arpTable->table[i].bPhysAddr[0], arpTable->table[i].bPhysAddr[1],
                            arpTable->table[i].bPhysAddr[2], arpTable->table[i].bPhysAddr[3],
                            arpTable->table[i].bPhysAddr[4], arpTable->table[i].bPhysAddr[5]);
                    break;
                }
            }
        }
        free(arpTable);
    }
    return recv_len;
}

void udp_get_host_ip(char *ip)
{
    char hostname[256];
    gethostname(hostname, sizeof(hostname));
    struct addrinfo *info;
    getaddrinfo(hostname, NULL, NULL, &info);
    inet_ntop(AF_INET, &((struct sockaddr_in *)info->ai_addr)->sin_addr, ip, INET_ADDRSTRLEN);
    freeaddrinfo(info);
}

void udp_get_host_mac(char *mac)
{
    IP_ADAPTER_INFO AdapterInfo[16];
    DWORD dwBufLen = sizeof(AdapterInfo);
    DWORD dwStatus = GetAdaptersInfo(AdapterInfo, &dwBufLen);
    if (dwStatus == ERROR_SUCCESS)
    {
        PIP_ADAPTER_INFO pAdapterInfo = AdapterInfo;
        while (pAdapterInfo) {
            if (pAdapterInfo->Type == MIB_IF_TYPE_ETHERNET) {
                sprintf(mac, "%02x:%02x:%02x:%02x:%02x:%02x",
                        pAdapterInfo->Address[0], pAdapterInfo->Address[1], pAdapterInfo->Address[2],
                        pAdapterInfo->Address[3], pAdapterInfo->Address[4], pAdapterInfo->Address[5]);
                break;
            }
            pAdapterInfo = pAdapterInfo->Next;
        }
    } else {
        // Handle error
    }
}

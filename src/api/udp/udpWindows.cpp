#include "udp.hpp"

#include "tools/logStream/logStream.hpp"

#include <assert.h>
#include <iphlpapi.h>
#include <stdint.h>
#include <stdio.h>
#include <winsock2.h>
#include <ws2tcpip.h>

#pragma comment(lib, "ws2_32.lib")
#pragma comment(lib, "iphlpapi.lib")

#define ARP_TABLE_SIZE 65536

SOCKET udp_socket;
struct sockaddr_in udp_addr;

void udp_init()
{
    WSADATA wsaData;
    if (WSAStartup(MAKEWORD(2, 2), &wsaData))
    {
        LogStream::cout << "Failed to initialize Winsock with error code : "
                        << WSAGetLastError() << LogStream::endl;
        exit(EXIT_FAILURE);
    }

    if ((udp_socket = socket(AF_INET, SOCK_DGRAM, 0)) == INVALID_SOCKET)
    {
        LogStream::cout << "Could not create socket : " << WSAGetLastError()
                        << LogStream::endl;
        exit(EXIT_FAILURE);
    }

    udp_addr.sin_family = AF_INET;
    udp_addr.sin_addr.s_addr = INADDR_ANY;
    udp_addr.sin_port = htons(UDP_DEFAULT_PORT);

    if (bind(udp_socket, (struct sockaddr *)&udp_addr, sizeof(udp_addr)))
    {
        LogStream::cout << "Bind failed with error code : " << WSAGetLastError()
                        << LogStream::endl;
        exit(EXIT_FAILURE);
    }
}

void udp_send(const char *data, int16_t len, const char *ip, int16_t port)
{
    struct sockaddr_in dest_addr;
    memset(&dest_addr, 0, sizeof(dest_addr));
    dest_addr.sin_family = AF_INET;
    dest_addr.sin_port = htons(port);
    dest_addr.sin_addr.S_un.S_addr = inet_addr(ip);
    int result = sendto(udp_socket,
                        data,
                        len,
                        0,
                        (struct sockaddr *)&dest_addr,
                        sizeof(dest_addr));
    if (result == SOCKET_ERROR)
    {
        LogStream::cout << "sendto() failed with error code : "
                        << WSAGetLastError() << LogStream::endl;
    }
}

void udp_send_broadcast(const char *data, int16_t len, int16_t port)
{
    // Get the host IP address
    char broadcastIp[INET_ADDRSTRLEN];
    udp_get_host_ip(broadcastIp);

    // Get the broadcast IP address
    uint8_t ipParts[4];
    sscanf(broadcastIp,
           "%hhu.%hhu.%hhu.%hhu",
           &ipParts[0],
           &ipParts[1],
           &ipParts[2],
           &ipParts[3]);
    ipParts[3] = 255;
    sprintf(broadcastIp,
            "%hhu.%hhu.%hhu.%hhu",
            ipParts[0],
            ipParts[1],
            ipParts[2],
            ipParts[3]);

    // Send the data to the broadcast IP
    udp_send(data, len, broadcastIp, port);
}

uint32_t udp_recv(char *data, int16_t len, char *ip, int16_t *port, char *mac)
{
    struct sockaddr_in src_addr;
    int src_addr_len = sizeof(src_addr);
    u_long mode = 1; // 1 to enable non-blocking mode
    ioctlsocket(udp_socket, FIONBIO, &mode);

    int received_len = recvfrom(
        udp_socket, data, len, 0, (struct sockaddr *)&src_addr, &src_addr_len);
    if (received_len == SOCKET_ERROR)
    {
        int error = WSAGetLastError();
        if (error == WSAEWOULDBLOCK)
        {
            return 0; // No data received
        }
        else
        {
            LogStream::cout << "recvfrom() failed with error code : " << error
                            << LogStream::endl;
            return 0;
        }
    }

    inet_ntop(AF_INET, &src_addr.sin_addr, ip, INET_ADDRSTRLEN);
    // Ignore the packet from machine itself
    char localIp[INET_ADDRSTRLEN];
    udp_get_host_ip(localIp);
    if (strcmp(ip, localIp) == 0)
    {
        return 0;
    }
    *port = ntohs(src_addr.sin_port);

    // Ignore the packet from machine itself
    char l_localIp[INET_ADDRSTRLEN];
    udp_get_host_ip(l_localIp);
    if (strcmp(ip, l_localIp) == 0)
    {
        return 0;
    }
    *port = ntohs(src_addr.sin_port);

    // Get the MAC address of the source IP
    ULONG macAddrLen = 6;
    BYTE macAddr[6];
    IPAddr src_ip = src_addr.sin_addr.s_addr;
    SendARP(src_ip, 0, macAddr, &macAddrLen);
    sprintf(mac,
            "%02x:%02x:%02x:%02x:%02x:%02x",
            macAddr[0],
            macAddr[1],
            macAddr[2],
            macAddr[3],
            macAddr[4],
            macAddr[5]);

    return received_len;
}

void udp_get_host_ip(char *ip)
{
    char hostname[256];
    gethostname(hostname, sizeof(hostname));
    struct addrinfo hints, *info;
    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_protocol = IPPROTO_TCP;
    getaddrinfo(hostname, NULL, &hints, &info);
    inet_ntop(AF_INET,
              &((struct sockaddr_in *)info->ai_addr)->sin_addr,
              ip,
              INET_ADDRSTRLEN);
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
        while (pAdapterInfo)
        {
            if (pAdapterInfo->Type == MIB_IF_TYPE_ETHERNET)
            {
                sprintf(mac,
                        "%02x:%02x:%02x:%02x:%02x:%02x",
                        pAdapterInfo->Address[0],
                        pAdapterInfo->Address[1],
                        pAdapterInfo->Address[2],
                        pAdapterInfo->Address[3],
                        pAdapterInfo->Address[4],
                        pAdapterInfo->Address[5]);
                break;
            }
            pAdapterInfo = pAdapterInfo->Next;
        }
    }
    else
    {
        // Handle error
    }
}

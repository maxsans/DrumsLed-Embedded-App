#include "udp.hpp"
#include <winsock2.h>
#include <ws2tcpip.h>
#include <atomic>
#include <cstdio>
#include <iphlpapi.h>
#include <thread>

#pragma comment(lib, "ws2_32.lib")
#pragma comment(lib, "iphlpapi.lib")

static udp_recv_callback_t udp_recv_callback = nullptr;
static SOCKET udp_socket = INVALID_SOCKET;
static std::atomic<bool> udp_running(false);
static std::thread udp_thread;

static void get_mac_from_arp(const char *ip, char *mac)
{
    ULONG mac_addr[2];
    ULONG mac_addr_len = 6;
    DWORD ret = SendARP(inet_addr(ip), 0, mac_addr, &mac_addr_len);

    if (ret == NO_ERROR && mac_addr_len == 6)
    {
        BYTE *mac_bytes = (BYTE *)mac_addr;
        sprintf(mac,
                "%02X:%02X:%02X:%02X:%02X:%02X",
                mac_bytes[0],
                mac_bytes[1],
                mac_bytes[2],
                mac_bytes[3],
                mac_bytes[4],
                mac_bytes[5]);
    }
    else
    {
        strcpy(mac, "00:00:00:00:00:00");
    }
}

static void udp_listen_thread()
{
    char buffer[1024];
    sockaddr_in client_addr;
    int client_addr_len = sizeof(client_addr);

    while (udp_running)
    {
        int bytes_received = recvfrom(udp_socket,
                                      buffer,
                                      sizeof(buffer) - 1,
                                      0,
                                      (sockaddr *)&client_addr,
                                      &client_addr_len);

        if (bytes_received > 0 && udp_recv_callback)
        {
            buffer[bytes_received] = '\0';
            char client_ip[INET_ADDRSTRLEN];
            char client_mac[18];
            inet_ntop(
                AF_INET, &client_addr.sin_addr, client_ip, INET_ADDRSTRLEN);
            get_mac_from_arp(client_ip, client_mac);
            udp_recv_callback(buffer,
                              bytes_received,
                              client_ip,
                              ntohs(client_addr.sin_port),
                              client_mac);
        }
        else if (bytes_received == SOCKET_ERROR)
        {
            int error = WSAGetLastError();
            if (error != WSAEWOULDBLOCK && udp_running)
            {
                // Handle error
                break;
            }
        }
    }
}

void udp_init(udp_recv_callback_t callback)
{
    udp_recv_callback = callback;

    WSADATA wsaData;
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
        return;

    udp_socket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if (udp_socket == INVALID_SOCKET)
    {
        WSACleanup();
        return;
    }

    // Set socket to non-blocking
    u_long mode = 1;
    ioctlsocket(udp_socket, FIONBIO, &mode);

    // Enable broadcast
    BOOL broadcast = TRUE;
    setsockopt(udp_socket,
               SOL_SOCKET,
               SO_BROADCAST,
               (char *)&broadcast,
               sizeof(broadcast));

    sockaddr_in server_addr;
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(UDP_DEFAULT_PORT);

    if (bind(udp_socket, (sockaddr *)&server_addr, sizeof(server_addr))
        == SOCKET_ERROR)
    {
        closesocket(udp_socket);
        WSACleanup();
        return;
    }

    udp_running = true;
    udp_thread = std::thread(udp_listen_thread);
}

void udp_send(const char *data, int16_t len, const char *ip, int16_t port)
{
    if (udp_socket == INVALID_SOCKET)
        return;

    sockaddr_in dest_addr;
    dest_addr.sin_family = AF_INET;
    inet_pton(AF_INET, ip, &dest_addr.sin_addr);
    dest_addr.sin_port = htons(port);

    sendto(udp_socket, data, len, 0, (sockaddr *)&dest_addr, sizeof(dest_addr));
}

void udp_send_broadcast(const char *data, int16_t len, int16_t port)
{
    if (udp_socket == INVALID_SOCKET)
        return;

    sockaddr_in broadcast_addr;
    broadcast_addr.sin_family = AF_INET;
    broadcast_addr.sin_addr.s_addr = INADDR_BROADCAST;
    broadcast_addr.sin_port = htons(port);

    sendto(udp_socket,
           data,
           len,
           0,
           (sockaddr *)&broadcast_addr,
           sizeof(broadcast_addr));
}

void udp_get_host_ip(char *ip)
{
    char hostname[256];
    if (gethostname(hostname, sizeof(hostname)) == 0)
    {
        struct addrinfo hints, *result;
        memset(&hints, 0, sizeof(hints));
        hints.ai_family = AF_INET;

        if (getaddrinfo(hostname, NULL, &hints, &result) == 0)
        {
            struct sockaddr_in *addr_in = (struct sockaddr_in *)result->ai_addr;
            inet_ntop(AF_INET, &addr_in->sin_addr, ip, INET_ADDRSTRLEN);
            freeaddrinfo(result);
        }
    }
}

void udp_get_host_mac(char *mac)
{
    IP_ADAPTER_INFO adapter_info[16];
    DWORD buf_len = sizeof(adapter_info);

    if (GetAdaptersInfo(adapter_info, &buf_len) == ERROR_SUCCESS)
    {
        PIP_ADAPTER_INFO adapter = adapter_info;
        while (adapter)
        {
            if (adapter->Type == MIB_IF_TYPE_ETHERNET)
            {
                sprintf(mac,
                        "%02X:%02X:%02X:%02X:%02X:%02X",
                        adapter->Address[0],
                        adapter->Address[1],
                        adapter->Address[2],
                        adapter->Address[3],
                        adapter->Address[4],
                        adapter->Address[5]);
                break;
            }
            adapter = adapter->Next;
        }
    }
}

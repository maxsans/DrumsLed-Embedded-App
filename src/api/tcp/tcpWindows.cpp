#include "tcp.hpp"
#include <iphlpapi.h>
#include <winsock2.h>
#include <ws2tcpip.h>

#pragma comment(lib, "ws2_32.lib")
#pragma comment(lib, "iphlpapi.lib")

static bool winsock_initialized = false;
static SOCKET listenSock = INVALID_SOCKET;

void tcp_init()
{
    if (!winsock_initialized)
    {
        WSADATA wsaData;
        if (WSAStartup(MAKEWORD(2, 2), &wsaData) == 0)
        {
            winsock_initialized = true;
        }
    }
    if (listenSock == INVALID_SOCKET)
    {
        listenSock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
        if (listenSock == INVALID_SOCKET)
            return;
        u_long mode = 1;
        ioctlsocket(listenSock, FIONBIO, &mode);
        sockaddr_in addr;
        addr.sin_family = AF_INET;
        addr.sin_port = htons(TCP_DEFAULT_PORT);
        addr.sin_addr.s_addr = INADDR_ANY;
        if (bind(listenSock, (sockaddr *)&addr, sizeof(addr)) == SOCKET_ERROR)
        {
            closesocket(listenSock);
            listenSock = INVALID_SOCKET;
            return;
        }
        if (listen(listenSock, 4) == SOCKET_ERROR)
        {
            closesocket(listenSock);
            listenSock = INVALID_SOCKET;
            return;
        }
    }
}

void tcp_send(const char *data, int16_t len, const char *ip, int16_t port)
{
    if (!winsock_initialized)
        tcp_init();
    SOCKET sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (sock == INVALID_SOCKET)
        return;
    sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_port = htons(port);
    inet_pton(AF_INET, ip, &addr.sin_addr);
    if (connect(sock, (sockaddr *)&addr, sizeof(addr)) == SOCKET_ERROR)
    {
        closesocket(sock);
        return;
    }
    send(sock, data, len, 0);
    closesocket(sock);
}

uint32_t tcp_recv(char *data, int16_t len, char *ip, int16_t *port, char *mac)
{
    static SOCKET clientSock = INVALID_SOCKET;
    static struct sockaddr_in clientAddr;
    static int clientLen = sizeof(clientAddr);

    if (!winsock_initialized || listenSock == INVALID_SOCKET)
        tcp_init();
    if (listenSock == INVALID_SOCKET)
        return 0;

    if (clientSock == INVALID_SOCKET)
    {
        fd_set readfds;
        FD_ZERO(&readfds);
        FD_SET(listenSock, &readfds);
        struct timeval tv = {0, 0};
        int sel = select(0, &readfds, NULL, NULL, &tv);
        if (sel <= 0)
            return 0;
        clientLen = sizeof(clientAddr);
        clientSock = accept(listenSock, (sockaddr *)&clientAddr, &clientLen);
        if (clientSock == INVALID_SOCKET)
            return 0;
    }

    int received = recv(clientSock, data, len, 0);
    if (received > 0)
    {
        if (ip)
            inet_ntop(AF_INET, &clientAddr.sin_addr, ip, 16);
        if (port)
            *port = ntohs(clientAddr.sin_port);
        if (mac)
            memset(mac, 0, 6);
        return received;
    }
    else if (received == 0
             || (received == SOCKET_ERROR
                 && WSAGetLastError() != WSAEWOULDBLOCK))
    {
        closesocket(clientSock);
        clientSock = INVALID_SOCKET;
        return 0;
    }
    return 0;
}

void tcp_get_host_ip(char *ip)
{
    if (!winsock_initialized)
        tcp_init();
    char hostname[256];
    if (gethostname(hostname, sizeof(hostname)) == SOCKET_ERROR)
    {
        if (ip)
            ip[0] = 0;
        return;
    }
    struct addrinfo hints = {0}, *res = NULL;
    hints.ai_family = AF_INET;
    if (getaddrinfo(hostname, NULL, &hints, &res) == 0 && res)
    {
        struct sockaddr_in *addr = (struct sockaddr_in *)res->ai_addr;
        inet_ntop(AF_INET, &addr->sin_addr, ip, 16);
        freeaddrinfo(res);
    }
    else
    {
        if (ip)
            ip[0] = 0;
    }
}

void tcp_get_host_mac(char *mac)
{
    if (!winsock_initialized)
        tcp_init();
    IP_ADAPTER_INFO AdapterInfo[16];
    DWORD buflen = sizeof(AdapterInfo);
    DWORD status = GetAdaptersInfo(AdapterInfo, &buflen);
    if (status == ERROR_SUCCESS)
    {
        PIP_ADAPTER_INFO pAdapterInfo = AdapterInfo;
        if (pAdapterInfo->AddressLength == 6 && mac)
        {
            memcpy(mac, pAdapterInfo->Address, 6);
            return;
        }
    }
    if (mac)
        memset(mac, 0, 6);
}

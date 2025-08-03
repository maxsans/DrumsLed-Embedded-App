#include "tcp.hpp"
#include <winsock2.h>
#include <ws2tcpip.h>
#include <iphlpapi.h>
#include <thread>
#include <vector>
#include <iostream>

#pragma comment(lib, "ws2_32.lib")
#pragma comment(lib, "iphlpapi.lib")

static tcp_recv_callback_t tcp_recv_callback = nullptr;
static SOCKET listen_socket = INVALID_SOCKET;
static std::thread server_thread;
static bool server_running = false;

void get_mac_from_arp(const char* ip, char* mac)
{
    strcpy(mac, "00:00:00:00:00:00"); // Default MAC
    
    ULONG buffer_size = 0;
    GetIpNetTable(nullptr, &buffer_size, FALSE);
    
    PMIB_IPNETTABLE arp_table = (PMIB_IPNETTABLE)malloc(buffer_size);
    if (arp_table == nullptr) return;
    
    if (GetIpNetTable(arp_table, &buffer_size, FALSE) == NO_ERROR)
    {
        DWORD target_ip = inet_addr(ip);
        for (DWORD i = 0; i < arp_table->dwNumEntries; i++)
        {
            if (arp_table->table[i].dwAddr == target_ip && 
                arp_table->table[i].dwPhysAddrLen == 6)
            {
                sprintf(mac, "%02X:%02X:%02X:%02X:%02X:%02X",
                    arp_table->table[i].bPhysAddr[0],
                    arp_table->table[i].bPhysAddr[1],
                    arp_table->table[i].bPhysAddr[2],
                    arp_table->table[i].bPhysAddr[3],
                    arp_table->table[i].bPhysAddr[4],
                    arp_table->table[i].bPhysAddr[5]);
                break;
            }
        }
    }
    
    free(arp_table);
}

void tcp_server_worker()
{
    while (server_running)
    {
        sockaddr_in client_addr;
        int client_addr_len = sizeof(client_addr);
        SOCKET client_socket = accept(listen_socket, (sockaddr*)&client_addr, &client_addr_len);
        if (client_socket == INVALID_SOCKET)
        {
            if (server_running) // Only log error if we're still supposed to be running
            {
                std::cerr << "Accept failed: " << WSAGetLastError() << std::endl;
            }
            continue;
        }

        // Handle client in separate thread
        std::thread([client_socket, client_addr]() {
            char buffer[1024];
            int bytes_received;
            char client_ip[INET_ADDRSTRLEN];
            char client_mac[18];
            
            inet_ntop(AF_INET, &client_addr.sin_addr, client_ip, INET_ADDRSTRLEN);
            int16_t client_port = ntohs(client_addr.sin_port);
            get_mac_from_arp(client_ip, client_mac);
            
            while ((bytes_received = recv(client_socket, buffer, sizeof(buffer) - 1, 0)) > 0)
            {
                buffer[bytes_received] = '\0';
                if (tcp_recv_callback)
                {
                    tcp_recv_callback(buffer, bytes_received, client_ip, client_port, client_mac);
                }
            }
            
            closesocket(client_socket);
        }).detach();
    }
}

void tcp_init(tcp_recv_callback_t recv_callback)
{
    tcp_recv_callback = recv_callback;
    
    // Initialize Winsock
    WSADATA wsaData;
    int result = WSAStartup(MAKEWORD(2, 2), &wsaData);
    if (result != 0)
    {
        std::cerr << "WSAStartup failed: " << result << std::endl;
        return;
    }

    // Create socket
    listen_socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (listen_socket == INVALID_SOCKET)
    {
        std::cerr << "Socket creation failed: " << WSAGetLastError() << std::endl;
        WSACleanup();
        return;
    }

    // Bind socket
    sockaddr_in service;
    service.sin_family = AF_INET;
    service.sin_addr.s_addr = INADDR_ANY;
    service.sin_port = htons(TCP_DEFAULT_PORT);

    if (bind(listen_socket, (SOCKADDR*)&service, sizeof(service)) == SOCKET_ERROR)
    {
        std::cerr << "Bind failed: " << WSAGetLastError() << std::endl;
        closesocket(listen_socket);
        WSACleanup();
        return;
    }

    // Start listening
    if (listen(listen_socket, SOMAXCONN) == SOCKET_ERROR)
    {
        std::cerr << "Listen failed: " << WSAGetLastError() << std::endl;
        closesocket(listen_socket);
        WSACleanup();
        return;
    }

    // Start server thread
    server_running = true;
    server_thread = std::thread(tcp_server_worker);
}

void tcp_send(const char *data, int16_t len, const char *ip, int16_t port)
{
    SOCKET client_socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (client_socket == INVALID_SOCKET)
    {
        std::cerr << "Client socket creation failed: " << WSAGetLastError() << std::endl;
        return;
    }

    sockaddr_in server_addr;
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(port);
    inet_pton(AF_INET, ip, &server_addr.sin_addr);

    if (connect(client_socket, (SOCKADDR*)&server_addr, sizeof(server_addr)) == SOCKET_ERROR)
    {
        std::cerr << "Connect failed: " << WSAGetLastError() << std::endl;
        closesocket(client_socket);
        return;
    }

    send(client_socket, data, len, 0);
    closesocket(client_socket);
}

void tcp_get_host_ip(char *ip)
{
    char hostname[256];
    if (gethostname(hostname, sizeof(hostname)) == SOCKET_ERROR)
    {
        strcpy(ip, "127.0.0.1");
        return;
    }

    struct addrinfo hints = {0};
    struct addrinfo *result = nullptr;
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;

    if (getaddrinfo(hostname, nullptr, &hints, &result) == 0)
    {
        struct sockaddr_in* addr_in = (struct sockaddr_in*)result->ai_addr;
        inet_ntop(AF_INET, &addr_in->sin_addr, ip, INET_ADDRSTRLEN);
        freeaddrinfo(result);
    }
    else
    {
        strcpy(ip, "127.0.0.1");
    }
}

void tcp_get_host_mac(char *mac)
{
    PIP_ADAPTER_INFO adapter_info = nullptr;
    ULONG buffer_size = 0;
    
    // Get required buffer size
    GetAdaptersInfo(nullptr, &buffer_size);
    adapter_info = (PIP_ADAPTER_INFO)malloc(buffer_size);
    
    if (GetAdaptersInfo(adapter_info, &buffer_size) == NO_ERROR)
    {
        PIP_ADAPTER_INFO adapter = adapter_info;
        while (adapter)
        {
            if (adapter->Type == MIB_IF_TYPE_ETHERNET && adapter->AddressLength == 6)
            {
                sprintf(mac, "%02X:%02X:%02X:%02X:%02X:%02X",
                    adapter->Address[0], adapter->Address[1], adapter->Address[2],
                    adapter->Address[3], adapter->Address[4], adapter->Address[5]);
                break;
            }
            adapter = adapter->Next;
        }
    }
    
    if (adapter_info)
    {
        free(adapter_info);
    }
    
    // Default MAC if not found
    if (strlen(mac) == 0)
    {
        strcpy(mac, "00:00:00:00:00:00");
    }
}
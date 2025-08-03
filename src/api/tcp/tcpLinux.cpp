#include "tcp.hpp"
#include <arpa/inet.h>
#include <cstring>
#include <ifaddrs.h>
#include <net/if.h>
#include <netdb.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <thread>
#include <unistd.h>

static tcp_recv_callback_t tcp_recv_callback = nullptr;
static int server_socket = -1;
static std::thread server_thread;
static bool server_running = false;

static void server_loop()
{
    struct sockaddr_in server_addr, client_addr;
    socklen_t client_len = sizeof(client_addr);

    server_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (server_socket < 0)
        return;

    int opt = 1;
    setsockopt(server_socket, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(TCP_DEFAULT_PORT);

    if (bind(
            server_socket, (struct sockaddr *)&server_addr, sizeof(server_addr))
        < 0)
    {
        close(server_socket);
        return;
    }

    if (listen(server_socket, 5) < 0)
    {
        close(server_socket);
        return;
    }

    while (server_running)
    {
        int client_socket = accept(
            server_socket, (struct sockaddr *)&client_addr, &client_len);
        if (client_socket < 0)
            continue;

        char buffer[1024];
        int bytes_received = recv(client_socket, buffer, sizeof(buffer) - 1, 0);

        if (bytes_received > 0 && tcp_recv_callback)
        {
            buffer[bytes_received] = '\0';
            char client_ip[INET_ADDRSTRLEN];
            inet_ntop(
                AF_INET, &client_addr.sin_addr, client_ip, INET_ADDRSTRLEN);
            int16_t client_port = ntohs(client_addr.sin_port);
            char mac[18]
                = "00:00:00:00:00:00"; // MAC not available from TCP connection

            tcp_recv_callback(
                buffer, bytes_received, client_ip, client_port, mac);
        }

        close(client_socket);
    }
}

void tcp_init(tcp_recv_callback_t recv_callback)
{
    tcp_recv_callback = recv_callback;
    server_running = true;
    server_thread = std::thread(server_loop);
}

void tcp_send(const char *data, int16_t len, const char *ip, int16_t port)
{
    int client_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (client_socket < 0)
        return;

    struct sockaddr_in server_addr;
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(port);
    inet_pton(AF_INET, ip, &server_addr.sin_addr);

    if (connect(
            client_socket, (struct sockaddr *)&server_addr, sizeof(server_addr))
        == 0)
    {
        send(client_socket, data, len, 0);
    }

    close(client_socket);
}

void tcp_get_host_ip(char *ip)
{
    struct ifaddrs *ifaddr, *ifa;

    if (getifaddrs(&ifaddr) == -1)
    {
        strcpy(ip, "127.0.0.1");
        return;
    }

    for (ifa = ifaddr; ifa != nullptr; ifa = ifa->ifa_next)
    {
        if (ifa->ifa_addr == nullptr)
            continue;

        if (ifa->ifa_addr->sa_family == AF_INET
            && strcmp(ifa->ifa_name, "lo") != 0)
        {
            struct sockaddr_in *addr_in = (struct sockaddr_in *)ifa->ifa_addr;
            strcpy(ip, inet_ntoa(addr_in->sin_addr));
            freeifaddrs(ifaddr);
            return;
        }
    }

    strcpy(ip, "127.0.0.1");
    freeifaddrs(ifaddr);
}

void tcp_get_host_mac(char *mac)
{
    struct ifaddrs *ifaddr, *ifa;

    if (getifaddrs(&ifaddr) == -1)
    {
        strcpy(mac, "00:00:00:00:00:00");
        return;
    }

    for (ifa = ifaddr; ifa != nullptr; ifa = ifa->ifa_next)
    {
        if (ifa->ifa_addr == nullptr)
            continue;

        if (strcmp(ifa->ifa_name, "lo") != 0)
        {
            // For simplicity, we'll use a placeholder MAC address
            // Getting the actual MAC requires additional system calls
            strcpy(mac, "02:00:00:00:00:00");
            freeifaddrs(ifaddr);
            return;
        }
    }

    strcpy(mac, "00:00:00:00:00:00");
    freeifaddrs(ifaddr);
}

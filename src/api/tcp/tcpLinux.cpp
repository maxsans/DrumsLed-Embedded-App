#include "tcp.hpp"
#include <arpa/inet.h>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <ifaddrs.h>
#include <net/if.h>
#include <netdb.h>
#include <netinet/in.h>
#include <netpacket/packet.h>
#include <sys/ioctl.h>
#include <sys/socket.h>
#include <unistd.h>

void tcp_init()
{
    // No initialization needed for Linux sockets
}

void tcp_send(const char *data, int16_t len, const char *ip, int16_t port)
{
    int sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock < 0)
        return;

    sockaddr_in addr;
    memset(&addr, 0, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_port = htons(port);
    inet_pton(AF_INET, ip, &addr.sin_addr);

    if (connect(sock, (sockaddr *)&addr, sizeof(addr)) == 0)
    {
        send(sock, data, len, 0);
    }
    close(sock);
}

uint32_t tcp_recv(char *data, int16_t len, char *ip, int16_t *port, char *mac)
{
    // Listen on TCP_DEFAULT_PORT and accept one connection
    int listen_sock = socket(AF_INET, SOCK_STREAM, 0);
    if (listen_sock < 0)
        return 0;

    sockaddr_in addr;
    memset(&addr, 0, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = INADDR_ANY;
    addr.sin_port = htons(TCP_DEFAULT_PORT);

    if (bind(listen_sock, (sockaddr *)&addr, sizeof(addr)) < 0)
    {
        close(listen_sock);
        return 0;
    }

    socklen_t addrlen = sizeof(addr);
    if (getsockname(listen_sock, (sockaddr *)&addr, &addrlen) == 0)
    {
        if (port)
            *port = ntohs(addr.sin_port);
    }

    listen(listen_sock, 1);

    sockaddr_in client_addr;
    socklen_t client_len = sizeof(client_addr);
    int client_sock
        = accept(listen_sock, (sockaddr *)&client_addr, &client_len);
    if (client_sock < 0)
    {
        close(listen_sock);
        return 0;
    }

    if (ip)
        inet_ntop(AF_INET, &client_addr.sin_addr, ip, INET_ADDRSTRLEN);
    if (port)
        *port = ntohs(client_addr.sin_port);

    ssize_t rlen = recv(client_sock, data, len, 0);

    // Optionally get MAC address (not possible from TCP socket directly)
    if (mac)
        mac[0] = '\0';

    close(client_sock);
    close(listen_sock);

    return (rlen > 0) ? rlen : 0;
}

void tcp_get_host_ip(char *ip)
{
    struct ifaddrs *ifaddr, *ifa;
    if (getifaddrs(&ifaddr) == -1)
    {
        if (ip)
            ip[0] = '\0';
        return;
    }
    for (ifa = ifaddr; ifa; ifa = ifa->ifa_next)
    {
        if (!ifa->ifa_addr)
            continue;
        if (ifa->ifa_addr->sa_family == AF_INET
            && !(ifa->ifa_flags & IFF_LOOPBACK))
        {
            struct sockaddr_in *sa = (struct sockaddr_in *)ifa->ifa_addr;
            inet_ntop(AF_INET, &sa->sin_addr, ip, INET_ADDRSTRLEN);
            freeifaddrs(ifaddr);
            return;
        }
    }
    if (ip)
        ip[0] = '\0';
    freeifaddrs(ifaddr);
}

void tcp_get_host_mac(char *mac)
{
    struct ifaddrs *ifaddr, *ifa;
    if (getifaddrs(&ifaddr) == -1)
    {
        if (mac)
            mac[0] = '\0';
        return;
    }
    for (ifa = ifaddr; ifa; ifa = ifa->ifa_next)
    {
        if (!ifa->ifa_addr)
            continue;
        if (ifa->ifa_addr->sa_family == AF_PACKET
            && !(ifa->ifa_flags & IFF_LOOPBACK))
        {
            struct sockaddr_ll *s = (struct sockaddr_ll *)ifa->ifa_addr;
            if (mac)
            {
                snprintf(mac,
                         18,
                         "%02x:%02x:%02x:%02x:%02x:%02x",
                         s->sll_addr[0],
                         s->sll_addr[1],
                         s->sll_addr[2],
                         s->sll_addr[3],
                         s->sll_addr[4],
                         s->sll_addr[5]);
            }
            freeifaddrs(ifaddr);
            return;
        }
    }
    if (mac)
        mac[0] = '\0';
    freeifaddrs(ifaddr);
}

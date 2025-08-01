#include "udp.hpp"

#include <arpa/inet.h>
#include <assert.h>
#include <cstdio>
#include <cstring>
#include <fcntl.h>
#include <ifaddrs.h>
#include <linux/if_packet.h>
#include <net/if.h>
#include <net/if_arp.h>
#include <netdb.h>
#include <netinet/in.h>
#include <pthread.h>
#include <stdint.h>
#include <sys/ioctl.h>
#include <sys/socket.h>
#include <unistd.h>

#include <string>
#include <vector>

static pthread_t udp_recv_thread;
static udp_recv_callback_t udp_recv_callback = nullptr;

static const char *udp_broadcast_ip = "192.168.1.255";

static int udp_sock = -1;

static void get_mac_from_ip(const char *ip, char *mac)
{
    struct arpreq req;
    struct sockaddr_in *sin;
    int sd = socket(AF_INET, SOCK_DGRAM, 0);
    memset(&req, 0, sizeof(req));
    sin = (struct sockaddr_in *)&req.arp_pa;
    sin->sin_family = AF_INET;
    inet_pton(AF_INET, ip, &sin->sin_addr);
    strcpy(req.arp_dev, "eth0"); // adapt if needed
    if (ioctl(sd, SIOCGARP, &req) == 0)
    {
        unsigned char *hw = (unsigned char *)req.arp_ha.sa_data;
        sprintf(mac,
                "%02x:%02x:%02x:%02x:%02x:%02x",
                hw[0],
                hw[1],
                hw[2],
                hw[3],
                hw[4],
                hw[5]);
    }
    else
    {
        strcpy(mac, "00:00:00:00:00:00");
    }
    close(sd);
}

static void get_host_ip_internal(char *ip)
{
    struct ifaddrs *ifaddr, *ifa;
    getifaddrs(&ifaddr);
    for (ifa = ifaddr; ifa != NULL; ifa = ifa->ifa_next)
    {
        if (ifa->ifa_addr && ifa->ifa_addr->sa_family == AF_INET
            && strcmp(ifa->ifa_name, "lo") != 0)
        {
            struct sockaddr_in *sa = (struct sockaddr_in *)ifa->ifa_addr;
            strcpy(ip, inet_ntoa(sa->sin_addr));
            break;
        }
    }
    freeifaddrs(ifaddr);
}

static void get_host_mac_internal(char *mac)
{
    struct ifaddrs *ifaddr, *ifa;
    getifaddrs(&ifaddr);
    for (ifa = ifaddr; ifa != NULL; ifa = ifa->ifa_next)
    {
        if (ifa->ifa_addr && ifa->ifa_addr->sa_family == AF_PACKET
            && strcmp(ifa->ifa_name, "lo") != 0)
        {
            struct sockaddr_ll *s = (struct sockaddr_ll *)ifa->ifa_addr;
            sprintf(mac,
                    "%02x:%02x:%02x:%02x:%02x:%02x",
                    s->sll_addr[0],
                    s->sll_addr[1],
                    s->sll_addr[2],
                    s->sll_addr[3],
                    s->sll_addr[4],
                    s->sll_addr[5]);
            break;
        }
    }
    freeifaddrs(ifaddr);
}

static void get_local_ips(std::vector<std::string> &ips)
{
    struct ifaddrs *ifaddr, *ifa;
    getifaddrs(&ifaddr);
    for (ifa = ifaddr; ifa != NULL; ifa = ifa->ifa_next)
    {
        if (ifa->ifa_addr && ifa->ifa_addr->sa_family == AF_INET)
        {
            struct sockaddr_in *sa = (struct sockaddr_in *)ifa->ifa_addr;
            std::string ip = inet_ntoa(sa->sin_addr);
            ips.push_back(ip);
        }
    }
    freeifaddrs(ifaddr);

    // Manually adds the bridge Docker IP
    ips.push_back("172.17.0.1");
}

static void *udp_recv_loop(void *arg)
{
    char buf[2048];
    struct sockaddr_in src_addr;
    socklen_t addrlen = sizeof(src_addr);

    std::vector<std::string> local_ips;
    get_local_ips(local_ips);

    while (1)
    {
        int n = recvfrom(udp_sock,
                         buf,
                         sizeof(buf),
                         0,
                         (struct sockaddr *)&src_addr,
                         &addrlen);
        if (n > 0 && udp_recv_callback)
        {
            char ip[64] = {0};
            char mac[32] = {0};
            strcpy(ip, inet_ntoa(src_addr.sin_addr));
            // Ignore packets from any local IP (including bridge/docker)
            // bool from_self = false;
            // for (const auto &local_ip : local_ips)
            // {
            //     if (strcmp(ip, local_ip.c_str()) == 0)
            //     {
            //         from_self = true;
            //         break;
            //     }
            // }
            // if (from_self)
            // {
            //     continue;
            // }
            get_mac_from_ip(ip, mac);
            printf("UDP packet received from %s:%d (%s): %.*s\n",
                   ip,
                   ntohs(src_addr.sin_port),
                   mac,
                   n,
                   buf);
            udp_recv_callback(buf, n, ip, ntohs(src_addr.sin_port), mac);
        }
    }
    return nullptr;
}

void udp_init(udp_recv_callback_t callback)
{
    udp_recv_callback = callback;

    udp_sock = socket(AF_INET, SOCK_DGRAM, 0);
    int opt = 1;
    setsockopt(udp_sock, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));
    setsockopt(udp_sock, SOL_SOCKET, SO_BROADCAST, &opt, sizeof(opt));

    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = INADDR_ANY;
    addr.sin_port = htons(UDP_DEFAULT_PORT);

    bind(udp_sock, (struct sockaddr *)&addr, sizeof(addr));

    pthread_create(&udp_recv_thread, nullptr, udp_recv_loop, nullptr);
}

void udp_send(const char *data, int16_t len, const char *ip, int16_t port)
{
    int sock = socket(AF_INET, SOCK_DGRAM, 0);
    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_port = htons(port);
    inet_pton(AF_INET, ip, &addr.sin_addr);
    sendto(sock, data, len, 0, (struct sockaddr *)&addr, sizeof(addr));
    close(sock);
}

void udp_send_broadcast(const char *data, int16_t len, int16_t port)
{
    return; // Do not commit !
    int sock = socket(AF_INET, SOCK_DGRAM, 0);
    int opt = 1;
    setsockopt(sock, SOL_SOCKET, SO_BROADCAST, &opt, sizeof(opt));
    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_port = htons(port);
    inet_pton(AF_INET, udp_broadcast_ip, &addr.sin_addr);
    sendto(sock, data, len, 0, (struct sockaddr *)&addr, sizeof(addr));
    close(sock);
}

void udp_get_host_ip(char *ip)
{
    get_host_ip_internal(ip);
}

void udp_get_host_mac(char *mac)
{
    get_host_mac_internal(mac);
}

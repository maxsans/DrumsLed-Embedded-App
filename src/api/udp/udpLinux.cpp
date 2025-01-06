#include "udp.h"

#include <stdint.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <unistd.h>
#include <cstring>
#include <netdb.h>
#include <assert.h>
#include <ifaddrs.h>
#include <net/if.h>
#include <sys/ioctl.h>
#include <net/if_arp.h>
#include <linux/if_packet.h>
#include <cstdio>

int udp_socket;
struct sockaddr_in udp_addr;

void udp_init()
{
    udp_socket = socket(AF_INET, SOCK_DGRAM, 0);
    assert(udp_socket != -1);
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
    int broadcastEnable = 1;
    setsockopt(udp_socket, SOL_SOCKET, SO_BROADCAST, &broadcastEnable, sizeof(broadcastEnable));
    udp_addr.sin_port = htons(port);
    udp_addr.sin_addr.s_addr = htonl(INADDR_BROADCAST);
    sendto(udp_socket, data, len, 0, (struct sockaddr *)&udp_addr, sizeof(udp_addr));
}

uint32_t udp_recv(char *data, int16_t len, char *ip, int16_t *port, char *mac)
{
    struct sockaddr_in src_addr;
    socklen_t addr_len = sizeof(src_addr);
    int recv_len = recvfrom(udp_socket, data, len, 0, (struct sockaddr *)&src_addr, &addr_len);
    if (recv_len > 0) {
        inet_ntop(AF_INET, &src_addr.sin_addr, ip, INET_ADDRSTRLEN);
        *port = ntohs(src_addr.sin_port);

        // Retrieve MAC address from ARP table
        struct arpreq req;
        struct sockaddr_in *sin;
        memset(&req, 0, sizeof(struct arpreq));
        sin = (struct sockaddr_in *)&req.arp_pa;
        sin->sin_family = AF_INET;
        sin->sin_addr = src_addr.sin_addr;
        int s = socket(AF_INET, SOCK_DGRAM, 0);
        if (ioctl(s, SIOCGARP, &req) == 0) {
            unsigned char *hwaddr = (unsigned char *)req.arp_ha.sa_data;
            sprintf(mac, "%02x:%02x:%02x:%02x:%02x:%02x",
                    hwaddr[0], hwaddr[1], hwaddr[2],
                    hwaddr[3], hwaddr[4], hwaddr[5]);
        }
        close(s);
    }
    return recv_len;
}

void udp_get_host_ip(char *ip)
{
    char host[256];
    if (gethostname(host, sizeof(host)) == 0) {
        struct hostent *host_entry = gethostbyname(host);
        assert(host_entry);
        if (host_entry) {
            strcpy(ip, inet_ntoa(*(struct in_addr*)host_entry->h_addr_list[0]));
        }
    } else {
        assert(false);
    }
}

void udp_get_host_mac(char *mac)
{
    struct ifaddrs *ifaddr, *ifa;
    if (getifaddrs(&ifaddr) == 0) {
        for (ifa = ifaddr; ifa != NULL; ifa = ifa->ifa_next) {
            if (ifa->ifa_addr && ifa->ifa_addr->sa_family == AF_PACKET) {
                struct sockaddr_ll *s = (struct sockaddr_ll*)ifa->ifa_addr;
                if (s->sll_halen == 6) {
                    sprintf(mac, "%02x:%02x:%02x:%02x:%02x:%02x",
                            s->sll_addr[0], s->sll_addr[1], s->sll_addr[2],
                            s->sll_addr[3], s->sll_addr[4], s->sll_addr[5]);
                    break;
                }
            }
        }
        freeifaddrs(ifaddr);
    } else {
        // Handle error
    }
}

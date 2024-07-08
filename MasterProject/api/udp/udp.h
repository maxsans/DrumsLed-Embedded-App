#ifndef __UDP_H__
#define __UDP_H__

#include "udpPacket.h"

#include <winsock2.h>
#include <stdio.h>
#include <stdint.h>

class udp
{
    private:
    SOCKET s;
    struct sockaddr_in si_other;
    int slen , recv_len;
    udpPacket m_udpPacket;
    void recv();

    public:
    udp();
    void init();
    void close();
    void send(char *ip_addr, char *msg, uint32_t msgSize);
    void sendbroadcast(char *msg, uint32_t msgSize);
    udpPacket *getPacket();
};

extern udp g_udp;

#endif

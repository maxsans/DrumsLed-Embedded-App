#ifndef __UDP_H__
#define __UDP_H__

#include <winsock2.h>
#include <stdio.h>
#include <stdint.h>

#define BUFLEN 512	//Max length of buffer

class udp
{
    private:
    SOCKET s;
    struct sockaddr_in si_other;
    int slen , recv_len;
    char buf[BUFLEN];

    public:
    udp();
    void init();
    void close();
    void send(char *ip_addr, char *msg, uint32_t msgSize);
    void sendbroadcast(char *msg, uint32_t msgSize);
    void recv();
};

extern udp g_udp;

#endif

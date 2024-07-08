#ifndef __UDP_PACKET_H__
#define __UDP_PACKET_H__

#define BUFLEN 512

class udpPacket
{
    private:
        char m_ip[16];
        char m_packet[BUFLEN];

    public:
        udpPacket();
        ~udpPacket();
        void setPacket(char *packet, char *ip);
        void setIp(char *ip);
        char *getPacket();
        char *getIp();
};

#endif

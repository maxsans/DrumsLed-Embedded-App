#ifndef __UDP_PACKET_H__
#define __UDP_PACKET_H__

#include "client.h"

#define BUFLEN 512

typedef enum
{
    PACKET_TYPE_INIT = 10,
    PACKET_TYPE_RGB,
    PACKET_TYPE_ADC
} packet_type_t;

class udpPacket
{
    private:
        Client m_client;
        char m_data[BUFLEN];
        uint32_t m_len;

    public:
        udpPacket();
        udpPacket(Client client, char *packet, uint32_t len);
        ~udpPacket();
        void setPacket(Client client, char *packet, uint32_t len);
        void setClient(Client client);
        void setData(char *packet, uint32_t len);
        void setLen(uint32_t len);
        char *getData();
        uint32_t getLen();
        Client getClient();
};

#endif

#ifndef __UDP_PARSER_H__
#define __UDP_PARSER_H__

typedef enum
{
    PACKET_TYPE_INIT = 1,
    PACKET_TYPE_RGB,
    PACKET_TYPE_ADC
} packet_type_t;

class udpParser
{
    private:

    protected:

    public:
        udpParser();
        ~udpParser();
        void parseUdp();
        void parseUdp(char* packet, char* ip);
};

extern udpParser g_udpParser;

#endif

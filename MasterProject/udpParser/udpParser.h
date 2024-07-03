#ifndef __UDP_PARSER_H__
#define __UDP_PARSER_H__

class udpParser
{
    private:

    protected:

    public:
        udpParser();
        ~udpParser();
        void parseUdp(char* packet, char* ip);
};

extern udpParser g_udpParser;

#endif

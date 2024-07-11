#ifndef __UDP_PARSER_H__
#define __UDP_PARSER_H__

#include "session.h"

typedef enum
{
    PACKET_TYPE_INIT = 10,
    PACKET_TYPE_RGB,
    PACKET_TYPE_ADC
} packet_type_t;

class udpParser
{
    private:
        session *m_currentSession;

    public:
        udpParser();
        udpParser(session *currentSession);
        ~udpParser();
        void setCurrentSession(session *currentSession);
        void parseUdp();
        void parseUdp(char* packet, char* ip);
};

extern udpParser g_udpParser;

#endif

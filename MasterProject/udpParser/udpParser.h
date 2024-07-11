#ifndef __UDP_PARSER_H__
#define __UDP_PARSER_H__

#include "session.h"
#include "udpPacket.h"


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

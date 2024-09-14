#ifndef __UDP_PARSER_H__
#define __UDP_PARSER_H__

#include "session.h"
#include "udpPacket.h"
#include "client.h"

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
        void parseUdp(udpPacket *packet);
};

extern udpParser g_udpParser;

#endif

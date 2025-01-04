#ifndef __UDP_PARSER_H__
#define __UDP_PARSER_H__

#include "session/session.h"
#include "network/udp/udpPacket.h"
#include "network/client/client.h"

/**
 * @brief Class to parse UDP packets.
 */
class udpParser
{
    private:
        session *m_currentSession;

    public:
        udpParser();
        udpParser(session *currentSession);
        ~udpParser();
        /**
         * @brief Set the current session.
         *
         * @param currentSession The current session.
         */
        void setCurrentSession(session *currentSession);
        /**
         * @brief Parse UDP if there is one to parse.
         */
        void parseUdp();
        /**
         * @brief Parse a specific UDP packet.
         *
         * @param packet The UDP packet to parse.
         */
        void parseUdp(udpPacket *packet);
};

extern udpParser g_udpParser;

#endif

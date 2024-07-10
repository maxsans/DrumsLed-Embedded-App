
#include "udp.h"
#include "udpParser.h"
#include "terminal.h"
#include "session.h"

#include <stdio.h>

int main()
{
    g_udp.init();

    // Create a session
    session l_session;
    g_udpParser.setCurrentSession(&l_session);
    terminal().setCurrentSession(&l_session);

    while(1)
    {
        g_udpParser.parseUdp();
        terminal().process();
        l_session.process();
    }
    g_udp.close();
    return 0;
}

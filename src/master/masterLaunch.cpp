#include "launch.h"
#include "network/udpParser/udpParser.h"
#include "session/session.h"
#include "tools/timeTools/periodicCallsMs.h"

#include <iostream>

void launch()
{
    std::cout << "Master Start !" << std::endl;

    // Create a session
    session l_session;
    g_udpParser.setCurrentSession(&l_session);
    // TODO: Move terminal from old project and enable this line
    // terminal::setCurrentSession(&l_session);

    while(1)
    {
        g_udpParser.parseUdp();
        // TODO: Move terminal from old project and enable this line
        // terminal::process();
        l_session.process();
        periodicCallsMs::processAll();
    }
}

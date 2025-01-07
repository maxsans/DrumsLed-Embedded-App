#include "launch.h"
#include "api/udp/udp.h"
#include "network/udpParser/udpParser.h"
#include "session/session.h"
#include "tools/timeTools/periodicCallsMs.h"

#include <iostream>

void launch()
{
    std::cout << "Master Start !" << std::endl;

    /* Initialization */
    // Initialize the udp api
    udp_init();

    // Print the host IP address and MAC address
    char l_ip[16];
    udp_get_host_ip(l_ip);
    std::cout << "Host IP: " << l_ip << std::endl;
    char l_mac[18];
    udp_get_host_mac(l_mac);
    std::cout << "Host MAC: " << l_mac << std::endl;

    // Create a session
    session l_session;
    // TODO: Move terminal from old project and enable this line
    // terminal::setCurrentSession(&l_session);

    while(1)
    {
        // Process the udp packets
        UdpParser::process();
        // TODO: Move terminal from old project and enable this line
        // terminal::process();
        // Process the session
        l_session.process();
        periodicCallsMs::processAll();
    }
}

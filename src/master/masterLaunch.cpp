#include "launch.h"
#include "api/udp/udp.h"
#include "api/logs/logStream.h"
#include "network/udpParser/udpParser.h"
#include "session/session.h"
#include "tools/timeTools/periodicCallsMs.h"

void launch()
{
    LogStream() << "Master started" << LogStream::endl;

    /* Initialization */
    // Initialize the udp api
    udp_init();

    // Print the host IP address and MAC address
    char l_ip[16];
    udp_get_host_ip(l_ip);
    LogStream() << "Host IP: " << l_ip << LogStream::endl;
    char l_mac[18];
    udp_get_host_mac(l_mac);
    LogStream() << "Host MAC: " << l_mac << LogStream::endl;

    // Create a session
    Session l_session;

    // TODO: Move terminal from old project and enable this line
    // terminal::setCurrentSession(&l_session);

    while(1)
    {
        periodicCallsMs::processAll();
        UdpParser::process();
        ModuleManager::process();
        // TODO: Move terminal from old project and enable this line
        // terminal::process();
        l_session.process();
    }
}

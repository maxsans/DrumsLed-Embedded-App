#include "launch.h"
#include "api/udp/udp.h"
#include "tools/logStream/logStream.h"
#include "network/interCom/interComParser/interComParser.hpp"
#include "network/interCom/interMsgList/interMsgExample/interMsgExample.hpp"
#include "session/session.h"
#include "tools/timeTools/periodicCallsMs.h"
#include "api/tcp/tcp.h"

void launch()
{
    LogStream::cout << "Master started" << LogStream::endl;

    /* Initialization */
    // Initialize the udp api
    udp_init();

    // Print the host IP address and MAC address
    char l_ip[16];
    udp_get_host_ip(l_ip);
    LogStream::cout << "Host IP: " << l_ip << LogStream::endl;
    char l_mac[18];
    udp_get_host_mac(l_mac);
    LogStream::cout << "Host MAC: " << l_mac << LogStream::endl;

    // Initialize some stuff
    ModuleManager::init();
    Learning::init();

    // Create a session
    Session l_session;

    // TODO: Move terminal from old project and enable this line
    // terminal::setCurrentSession(&l_session);

    tcp_init();

    // Initialize the interComParser
    InterComParser l_interComParser;
    InterComParser::registerCallback(InterMsgId::Example, [](const Client &client, InterMsg &msg) {
        LogStream::cout << "Received Example message from " << client.getIP().getIpString()
        << " with MAC: " << client.getMAC().getMacString()
        << " and message data: " << ((InterMsgExample &)msg).getExampleData()
        << LogStream::endl;
    });

    while(1)
    {
        periodicCallsMs::processAll();
        ModuleManager::process();
        // TODO: Move terminal from old project and enable this line
        // terminal::process();
        l_session.process();
    }
}

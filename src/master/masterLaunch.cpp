#include "launch.hpp"
#include "api/udp/udp.hpp"
#include "micro/learning/learning.hpp"
#include "modules/moduleManager.hpp"
#include "network/interCom/interComParser/interComParser.hpp"
#include "network/interCom/interMsgList/interMsgExample/interMsgExample.hpp"
#include "session/session.hpp"
#include "tools/async/async.hpp"
#include "tools/logStream/logStream.hpp"
#include "tools/term/term.hpp"
#include "tools/timeTools/periodicCallsMs.hpp"

void launch()
{
    LogStream::cout << "Master started" << LogStream::endl;

    /* Initialization */

    // Initialize the terminal
    Term::init();

    // Print the host IP address and MAC address
    char l_ip[16];
    udp_get_host_ip(l_ip);
    LogStream::cout << "Host IP: " << l_ip << LogStream::endl;
    char l_mac[18];
    udp_get_host_mac(l_mac);
    LogStream::cout << "Host MAC: " << l_mac << LogStream::endl;

    // Create a session
    Session l_session(true);

    // Initialize the interComParser
    InterComParser l_interComParser;
    InterComParser::registerCallback(
        InterMsgId::Example,
        [](const Client &client, InterMsg &msg, void *object) {
            LogStream::cout << "Received Example message from "
                            << client.getIP().getIpString()
                            << " with MAC: " << client.getMAC().getMacString()
                            << " and message data: "
                            << ((InterMsgExample &)msg).getExampleData()
                            << LogStream::endl;
        });

    while (1)
    {
        periodicCallsMs::processAll();
        Async::process();
    }
}

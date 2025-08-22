/**
 * @file masterLaunch.cpp
 * @author Cyprien Ménard
 * @date 2025-08-20
 */

#include "api/network/udp/udp.hpp"
#include "launch.hpp"
#include "micro/learning/learning.hpp"
#include "modules/moduleManager.hpp"
#include "network/interCom/interComParser/interComParser.hpp"
#include "network/interCom/interMsg/interMsgList/interMsgExample/interMsgExample.hpp"
#include "session/session.hpp"
#include "tools/logStream/logStream.hpp"
#include "tools/os/os.hpp"
#include "tools/term/term.hpp"

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
    InterComParser::registerDeserializer(
        new InterMsgExample([](const Client &client, InterMsg &msg) {
            LogStream::cout << "Received Example message from "
                            << client.getIP().getIpString()
                            << " with MAC: " << client.getMAC().getMacString()
                            << " and message data: "
                            << ((InterMsgExample &)msg).getExampleData()
                            << LogStream::endl;
        }));

    while (1)
    {
        OS::process();
    }
}

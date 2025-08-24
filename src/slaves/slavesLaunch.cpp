#include "api/target/common.hpp"
#include "api/wifi/wifi.hpp"
#include "kit/kit.hpp"
#include "launch.hpp"
#include "network/interCom/interComParser/interComParser.hpp"
#include "network/interCom/interMsgList/interMsgExample/interMsgExample.hpp"
#include "network/networkConfig.hpp"
#include "tools/logStream/logStream.hpp"
#include "tools/os/os.hpp"
#include "tools/timeTools/timeMs.hpp"

void launch()
{
    target_common_init();
    LogStream::cout << "Slaves start .." << LogStream::endl;

    wifi_set_sta(WIFI_SSID, WIFI_PASSWORD);
    wifi_init();
    Kit l_kit;
    LogStream::cout << "Slaves started" << LogStream::endl;

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
        target_process();
        OS::process();
    }
}

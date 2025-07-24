#include "launch.hpp"
#include "api/target/common.hpp"
#include "tools/logStream/logStream.hpp"
#include "tools/timeTools/periodicCallsMs.hpp"
#include "tools/async/async.hpp"
#include "api/wifi/wifi.hpp"
#include "api/udp/udp.hpp"
#include "api/tcp/tcp.hpp"
#include "network/networkConfig.hpp"
#include "tools/timeTools/timeMs.hpp"
#include "kit/kit.hpp"

static void init()
{
    target_common_init();
    LogStream::cout << "Slaves start .." << LogStream::endl;

    wifi_set_sta(WIFI_SSID, WIFI_PASSWORD);
    wifi_init();
    Kit::init();
    LogStream::cout << "Slaves started" << LogStream::endl;
}

static void process()
{
    target_process();
    periodicCallsMs::processAll();
    Async::process();
    // Init the udp on each wifi reconnexion
    static bool l_lastWifiStatus = false;
    bool l_newWifiState = is_wifi_connected();
    if (l_lastWifiStatus != l_newWifiState)
    {
        l_lastWifiStatus = l_newWifiState;
        udp_init();
        tcp_init();
        LogStream::cout << "Udp Initialized" << LogStream::endl;
    }
}

void launch()
{
    init();
    while (1)
    {
        process();
    }
}

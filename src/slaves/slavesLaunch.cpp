#include "launch
#include "api/target/common
#include "tools/logStream/logStream
#include "tools/timeTools/periodicCallsMs
#include "api/wifi/wifi
#include "api/adc/adc
#include "api/addrLed/addrLed
#include "api/udp/udp
#include "api/tcp/tcp
#include "network/networkConfig
#include "tools/timeTools/timeMs
#include "kit/kitConfig/kitConfigManager/kitConfigManager.hpp"

static void init()
{
    target_common_init();
    LogStream::cout << "Slaves start .." << LogStream::endl;

    adc_init();
    wifi_set_sta(WIFI_SSID, WIFI_PASSWORD);
    wifi_init();
    KitConfigManager::init();
    LogStream::cout << "Slaves started" << LogStream::endl;
}

static void process()
{
    target_process();
    periodicCallsMs::processAll();
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

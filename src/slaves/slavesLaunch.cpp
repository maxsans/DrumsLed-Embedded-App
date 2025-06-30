#include "launch.h"
#include "api/target/common.h"
#include "tools/logStream/logStream.h"
#include "tools/timeTools/periodicCallsMs.h"
#include "api/wifi/wifi.h"
#include "api/adc/adc.h"
#include "api/addrLed/addrLed.h"
#include "api/udp/udp.h"
#include "network/networkConfig.h"
#include "tools/timeTools/timeMs.h"

static void init()
{
    target_common_init();
    LogStream::cout << "Slaves start .." << LogStream::endl;

    adc_init();
    wifi_set_sta(WIFI_SSID, WIFI_PASSWORD);
    wifi_init();
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

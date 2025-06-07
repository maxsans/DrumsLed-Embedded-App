#include "launch.h"
#include "api/target/common.h"
#include "tools/logStream/logStream.h"
#include "api/wifi/wifi.h"
#include "api/adc/adc.h"
#include "api/addrLed/addrLed.h"
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
}

void launch()
{
    init();
    while (1)
    {
        process();
    }
}

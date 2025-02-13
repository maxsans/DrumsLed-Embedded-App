#include "launch.h"
#include "api/target/common.h"
#include "api/logs/logStream.h"
#include "api/wifi/wifi.h"
#include "api/adc/adc.h"
#include "network/networkConfig.h"

static bool initialized = false;

static void init()
{
    target_common_init();
    LogStream::cout << "Slaves start !" << LogStream::endl;

    target_common_init();

    adc_init();
    wifi_set_sta(WIFI_SSID, WIFI_PASSWORD);
    wifi_init();
}

static void process()
{
    target_process();
}

void launch()
{
    if (!initialized)
    {
        initialized = true;
        init();
    }
    process();
}

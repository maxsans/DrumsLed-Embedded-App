#include "launch.h"
#include "api/logs/logStream.h"
#include "api/wifi/wifi.h"
#include "api/adc/adc.h"
#include "network/networkConfig.h"

void launch()
{
    LogStream::cout << "Slaves start !" << LogStream::endl;

    adc_init();
    wifi_init();
    wifi_set_sta(WIFI_SSID, WIFI_PASSWORD);

    while(1);
}

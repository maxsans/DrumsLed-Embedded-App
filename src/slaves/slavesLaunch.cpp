#include "launch.h"
#include "api/logs/logStream.h"
#include "api/wifi/wifi.h"
#include "network/networkConfig.h"

void launch()
{
    LogStream() << "Slaves start !" << LogStream::endl;

    wifi_init();
    wifi_set_sta(WIFI_SSID, WIFI_PASSWORD);
}

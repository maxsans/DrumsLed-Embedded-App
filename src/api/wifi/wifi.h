#ifndef __WIFI_H__
#define __WIFI_H__

/**
 * @brief Function to initialize the wifi module
 *
 */
void wifi_init();

/**
 * @brief Function to set the wifi in station mode
 *
 * @param ssid The ssid of the wifi network
 * @param password The password of the wifi network
 */
void wifi_set_sta(const char *ssid, const char *password);

#endif // __WIFI_H__

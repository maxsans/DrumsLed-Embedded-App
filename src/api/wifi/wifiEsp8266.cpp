/**
 * @file wifiEsp8266.cpp
 * @author Cyprien M�nard
 * @brief This is an adaptation of the wifi example from the ESP8266 RTOS SDK.
 * https://github.com/espressif/esp-idf/blob/master/examples/wifi/getting_started/station/main/station_example_main.c
 * @version 0.1
 * @date 2025-02-13
 *
 * @copyright Copyright (c) 2025
 *
 */

#include "wifi

#include <string.h>
#include "freertos/FreeRTOS
#include "freertos/task
#include "freertos/event_groups
#include "esp_system
#include "esp_log
#include "esp_netif
#include "esp_event
#include "esp_wifi

#include "api/logs/logs

#include "lwip/err
#include "lwip/sys

/* FreeRTOS event group to signal when we are connected*/
static EventGroupHandle_t s_wifi_event_group;

/* The event group allows multiple bits for each event, but we only care about two events:
 * - we are connected to the AP with an IP
 * - we failed to connect after the maximum amount of retries */
#define WIFI_CONNECTED_BIT BIT0
#define WIFI_FAIL_BIT BIT1

#define SSID_MAX_LENGTH 32
#define PASSWORD_MAX_LENGTH 64

#define RECONNECT_DELAY_MS 5000

static char g_ssid[SSID_MAX_LENGTH];
static char g_password[PASSWORD_MAX_LENGTH];
static bool g_connected = false;

static void event_handler(void *arg, esp_event_base_t event_base,
                          int32_t event_id, void *event_data)
{
    if (event_base == WIFI_EVENT && event_id == WIFI_EVENT_STA_START)
    {
        esp_wifi_connect();
    }
    else if (event_base == WIFI_EVENT && event_id == WIFI_EVENT_STA_DISCONNECTED)
    {
        esp_wifi_connect();
        log("retry to connect to the AP\n");
    }
    else if (event_base == IP_EVENT && event_id == IP_EVENT_STA_GOT_IP)
    {
        ip_event_got_ip_t *event = (ip_event_got_ip_t *)event_data;
        log("got ip: %s \n",
            ip4addr_ntoa(&event->ip_info.ip));
        xEventGroupSetBits(s_wifi_event_group, WIFI_CONNECTED_BIT);
    }
}

static void wifi_connect_task(void *pvParameters)
{
    while (1)
    {
        s_wifi_event_group = xEventGroupCreate();

        tcpip_adapter_init();

        ESP_ERROR_CHECK(esp_event_loop_create_default());

        wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();
        ESP_ERROR_CHECK(esp_wifi_init(&cfg));

        ESP_ERROR_CHECK(esp_event_handler_register(WIFI_EVENT, ESP_EVENT_ANY_ID, &event_handler, NULL));
        ESP_ERROR_CHECK(esp_event_handler_register(IP_EVENT, IP_EVENT_STA_GOT_IP, &event_handler, NULL));

        wifi_config_t wifi_config;
        memset(&wifi_config, 0, sizeof(wifi_config));
        strcpy((char *)wifi_config.sta.ssid, g_ssid);
        strcpy((char *)wifi_config.sta.password, g_password);

        if (strlen((char *)wifi_config.sta.password))
        {
            wifi_config.sta.threshold.authmode = WIFI_AUTH_WPA2_PSK;
        }

        ESP_ERROR_CHECK(esp_wifi_set_mode(WIFI_MODE_STA));
        ESP_ERROR_CHECK(esp_wifi_set_config(ESP_IF_WIFI_STA, &wifi_config));
        ESP_ERROR_CHECK(esp_wifi_start());

        log("wifi_init_sta finished. Trying to connect to AP (ssid : %s, password : %s)\n", wifi_config.sta.ssid, wifi_config.sta.password);

        EventBits_t bits = xEventGroupWaitBits(s_wifi_event_group,
                                               WIFI_CONNECTED_BIT | WIFI_FAIL_BIT,
                                               pdFALSE,
                                               pdFALSE,
                                               portMAX_DELAY);

        if (bits & WIFI_CONNECTED_BIT)
        {
            log("connected to ap SSID: %s password: %s\n", g_ssid, g_password);
            g_connected = true;
            // Attendre la d�connexion
            xEventGroupClearBits(s_wifi_event_group, WIFI_FAIL_BIT);
            xEventGroupWaitBits(s_wifi_event_group, WIFI_FAIL_BIT, pdFALSE, pdFALSE, portMAX_DELAY);
            log("WiFi disconnected, will try to reconnect...\n");
            g_connected = false;
        }
        else if (bits & WIFI_FAIL_BIT)
        {
            g_connected = false;
            log("Failed to connect to SSID: %s, password: %s\n", g_ssid, g_password);
            vTaskDelay(RECONNECT_DELAY_MS / portTICK_PERIOD_MS);
        }
        else
        {
            g_connected = false;
            log("UNEXPECTED EVENT\n");
        }

        ESP_ERROR_CHECK(esp_event_handler_unregister(IP_EVENT, IP_EVENT_STA_GOT_IP, &event_handler));
        ESP_ERROR_CHECK(esp_event_handler_unregister(WIFI_EVENT, ESP_EVENT_ANY_ID, &event_handler));
        vEventGroupDelete(s_wifi_event_group);
    }
    vTaskDelete(NULL);
}

void wifi_init_sta(void)
{
    // Lance la connexion wifi dans une t�che ind�pendante
    xTaskCreate(&wifi_connect_task, "wifi_connect_task", 4096, NULL, 5, NULL);
}

void wifi_init()
{
    wifi_init_sta();
}

void wifi_set_sta(const char *ssid, const char *password)
{
    // Set the ssid and password
    strcpy(g_ssid, ssid);
    strcpy(g_password, password);
    log("Set wifi ssid: %s, password: %s\n", g_ssid, g_password);
}

bool is_wifi_connected()
{
    return g_connected;
}
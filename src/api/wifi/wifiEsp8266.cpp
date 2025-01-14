#include "wifi.h"

#include "esp_wifi.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_event.h"

#include <string.h>

void wifi_event_handler(void *arg, esp_event_base_t event_base, int32_t event_id, void *event_data)
{
    if (event_base == WIFI_EVENT && event_id == WIFI_EVENT_STA_DISCONNECTED) {
        esp_wifi_connect();
    }
}

void wifi_connect_task(void *pvParameters)
{
    const char **params = (const char **)pvParameters;
    const char *ssid = params[0];
    const char *password = params[1];

    wifi_config_t wifi_config = {};
    strcpy((char *)wifi_config.sta.ssid, ssid);
    strcpy((char *)wifi_config.sta.password, password);

    esp_wifi_set_mode(WIFI_MODE_STA);
    esp_wifi_set_config(ESP_IF_WIFI_STA, &wifi_config);
    esp_wifi_start();
    esp_wifi_connect();

    vTaskDelete(NULL); // Supprimer la tâche après la connexion
}

void wifi_init()
{
    // Initialize the wifi module
    wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();
    esp_wifi_init(&cfg);
    esp_wifi_set_storage(WIFI_STORAGE_RAM);
    esp_event_handler_register(WIFI_EVENT, WIFI_EVENT_STA_DISCONNECTED, &wifi_event_handler, NULL);
}

void wifi_set_sta(const char *ssid, const char *password)
{
    const char *params[] = {ssid, password};
    xTaskCreate(&wifi_connect_task, "wifi_connect_task", 4096, (void *)params, 5, NULL);
}

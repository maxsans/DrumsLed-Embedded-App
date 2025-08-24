#include "adc.hpp"
#include "driver/adc.h"
#include "esp_log.h"

static const char *TAG = "adc";
static bool adc_is_initialized = false;

void adc_init()
{
    // Prevent multiple initializations
    if (adc_is_initialized)
    {
        ESP_LOGW(TAG, "ADC already initialized, skipping");
        return;
    }

    // Initialize the ADC module
    adc_config_t adc_config;
    adc_config.mode = ADC_READ_TOUT_MODE;
    adc_config.clk_div = 8;

    esp_err_t ret = ::adc_init(&adc_config);
    if (ret == ESP_OK)
    {
        adc_is_initialized = true;
        ESP_LOGI(TAG, "ADC initialized successfully");
    }
    else
    {
        ESP_LOGE(TAG, "adc_init failed: %s", esp_err_to_name(ret));
    }
}

adc_measure_t adc_read()
{
    uint16_t adc_value = 0;
    esp_err_t ret = ::adc_read(&adc_value);
    if (ret == ESP_OK)
    {
        return (adc_measure_t)(adc_value & 0xFF); // Convert to 8-bit value
    }
    else
    {
        ESP_LOGE(TAG, "adc_read failed: %s", esp_err_to_name(ret));
        return 0; // Return 0 in case of error
    }
}

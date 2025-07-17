#include "adc.hpp"
#include "driver/adc.h"

void adc_init()
{
    // Initialize the ADC module
    adc_config_t adc_config;
    adc_config.mode = ADC_READ_TOUT_MODE;
    adc_config.clk_div = 8;
    adc_init(&adc_config);
}

adc_measure_t adc_read()
{
    uint16_t adc_value = 0;
    if (adc_read(&adc_value) == ESP_OK) {
        return adc_value;
    } else {
        return 0; // Return 0 in case of error
    }
}

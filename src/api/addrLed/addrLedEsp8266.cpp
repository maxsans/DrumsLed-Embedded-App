#include "addrLed.h"
#include "esp8266/gpio_struct.h"
#include "driver/gpio.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#include <assert.h>

#define WS2812_T0H_NS 350
#define WS2812_T1H_NS 700
#define WS2812_T0L_NS 800
#define WS2812_T1L_NS 600
#define WS2812_RESET_NS 50000

#define CPU_FREQ_MHZ 80

#define NB_GAMMA_CORRECTION 256

const uint8_t GAMMA_CORRECTION[NB_GAMMA_CORRECTION] =
{
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 1, 1, 1, 1, 2, 2, 2, 2, 2, 2, 2,
    2, 3, 3, 3, 3, 3, 3, 3, 4, 4, 4, 4, 4, 5, 5, 5,
    5, 6, 6, 6, 6, 7, 7, 7, 7, 8, 8, 8, 9, 9, 9, 10,
    10, 10, 11, 11, 11, 12, 12, 13, 13, 13, 14, 14, 15, 15, 16, 16,
    17, 17, 18, 18, 19, 19, 20, 20, 21, 21, 22, 22, 23, 24, 24, 25,
    25, 26, 27, 27, 28, 29, 29, 30, 31, 32, 32, 33, 34, 35, 35, 36,
    37, 38, 39, 39, 40, 41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 50,
    51, 52, 54, 55, 56, 57, 58, 59, 60, 61, 62, 63, 64, 66, 67, 68,
    69, 70, 72, 73, 74, 75, 77, 78, 79, 81, 82, 83, 85, 86, 87, 89,
    90, 92, 93, 95, 96, 98, 99, 101, 102, 104, 105, 107, 109, 110, 112, 114,
    115, 117, 119, 120, 122, 124, 126, 127, 129, 131, 133, 135, 137, 138, 140, 142,
    144, 146, 148, 150, 152, 154, 156, 158, 160, 162, 164, 167, 169, 171, 173, 175,
    177, 180, 182, 184, 186, 189, 191, 193, 196, 198, 200, 203, 205, 208, 210, 213,
    215, 218, 220, 223, 225, 228, 231, 233, 236, 239, 241, 244, 247, 249, 252, 255
};

static inline void delay_ns(uint32_t ns)
{
    uint32_t start = xthal_get_ccount();
    uint32_t cycles = ns * (CPU_FREQ_MHZ / 1000) / 1000;
    while (xthal_get_ccount() - start < cycles)
        ;
}

AddrLed::AddrLed(uint32_t num_leds, uint8_t pin) : m_num_leds(num_leds), m_pin(pin), m_pixels(num_leds)
{

}

AddrLed::~AddrLed()
{
    m_pixels.clear();
}

void AddrLed::init()
{
    gpio_config_t io_conf;
    io_conf.intr_type = GPIO_INTR_DISABLE;
    io_conf.mode = GPIO_MODE_OUTPUT;
    io_conf.pin_bit_mask = (1ULL << m_pin);
    io_conf.pull_down_en = GPIO_PULLDOWN_DISABLE;
    io_conf.pull_up_en = GPIO_PULLUP_DISABLE;
    gpio_config(&io_conf);
}

void AddrLed::setPixelColor(uint32_t index, addrLedColor_t color)
{
    if (index < m_num_leds)
    {
        m_pixels[index] = color;
    }
}

void AddrLed::fill(addrLedColor_t color)
{
    for (uint32_t i = 0; i < m_num_leds; i++)
    {
        m_pixels[i] = color;
    }
}

void AddrLed::fill(addrLedColor_t color, uint32_t start, uint32_t end)
{
    if (end == 0)
    {
        end = m_num_leds;
    }

    for (uint32_t i = start; i < end; i++)
    {
        m_pixels[i] = color;
    }
}

void AddrLed::show()
{
    uint32_t num_bytes = m_num_leds * 3;
    uint8_t *data = new uint8_t[num_bytes];
    assert(data != NULL);

    for (uint32_t i = 0; i < m_num_leds; i++)
    {
        data[i * 3 + 0] = GAMMA_CORRECTION[m_pixels[i].g];
        data[i * 3 + 1] = GAMMA_CORRECTION[m_pixels[i].r];
        data[i * 3 + 2] = GAMMA_CORRECTION[m_pixels[i].b];
    }

    for (uint32_t i = 0; i < num_bytes; i++)
    {
        uint8_t byte = data[i];
        for (uint8_t bit = 0; bit < 8; bit++)
        {
            if (byte & (1 << (7 - bit)))
            {
                gpio_set_level((gpio_num_t)m_pin, 1);
                delay_ns(WS2812_T1H_NS);
                gpio_set_level((gpio_num_t)m_pin, 0);
                delay_ns(WS2812_T1L_NS);
            }
            else
            {
                gpio_set_level((gpio_num_t)m_pin, 1);
                delay_ns(WS2812_T0H_NS);
                gpio_set_level((gpio_num_t)m_pin, 0);
                delay_ns(WS2812_T0L_NS);
            }
        }
    }

    delay_ns(WS2812_RESET_NS);
    delete[] data;
}

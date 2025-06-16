#include "addrLed.h"
#include "esp8266/gpio_struct.h"
#include "driver/gpio.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#include <assert.h>

#define F_CPU 160000000

#define CYCLES_800_T0H (F_CPU / 2500000) // 0.4us
#define CYCLES_800_T1H (F_CPU / 1250000) // 0.8us
#define CYCLES_800 (F_CPU / 800000)      // 1.25us per bit
#define WS2812_RESET_US 60

#define WS2812_MAX_PIXELS 1024 // Maximum number of pixels supported by this implementation

#define NB_GAMMA_CORRECTION 256

#define _BV(i) (1U << (i))

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
        215, 218, 220, 223, 225, 228, 231, 233, 236, 239, 241, 244, 247, 249, 252, 255};

void IRAM_ATTR bitbang_send_pixels_800(uint8_t *pixels, uint8_t *end, uint8_t pin)
{
    const uint32_t pinRegister = _BV(pin);
    uint8_t mask;
    uint8_t subpix;
    uint32_t cyclesStart;

    // trigger emediately
    cyclesStart = xthal_get_ccount() - CYCLES_800;
    do
    {
        subpix = *pixels++;
        for (mask = 0x80; mask != 0; mask >>= 1)
        {
            // do the checks here while we are waiting on time to pass
            uint32_t cyclesBit = ((subpix & mask)) ? CYCLES_800_T1H : CYCLES_800_T0H;
            uint32_t cyclesNext = cyclesStart;

            // after we have done as much work as needed for this next bit
            // now wait for the HIGH
            do
            {
                // cache and use this count so we don't incur another
                // instruction before we turn the bit high
                cyclesStart = xthal_get_ccount();
            } while ((cyclesStart - cyclesNext) < CYCLES_800);

            // set high
            GPIO_REG_WRITE(GPIO_OUT_W1TS_ADDRESS, pinRegister);

            // wait for the LOW
            do
            {
                cyclesNext = xthal_get_ccount();
            } while ((cyclesNext - cyclesStart) < cyclesBit);

            // set low
            GPIO_REG_WRITE(GPIO_OUT_W1TC_ADDRESS, pinRegister);
        }
    } while (pixels < end);
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

void AddrLed::fill(uint8_t r, uint8_t g, uint8_t b)
{
    addrLedColor_t color;
    color.r = r;
    color.g = g;
    color.b = b;
    fill(color);
}

void AddrLed::show()
{
    // Prepare buffer for WS2812: GRB order, gamma corrected
    uint8_t ws_buf[3 * WS2812_MAX_PIXELS];
    uint8_t *p = ws_buf;
    for (uint32_t i = 0; i < m_num_leds; ++i)
    {
        const addrLedColor_t &c = m_pixels[i];
        *p++ = GAMMA_CORRECTION[c.g];
        *p++ = GAMMA_CORRECTION[c.r];
        *p++ = GAMMA_CORRECTION[c.b];
    }
    // Disable interrupts !
    // This is important to ensure that the timing of the WS2812 signal is not disrupted
    taskENTER_CRITICAL();
    // Send the pixels
    bitbang_send_pixels_800(ws_buf, ws_buf + 3 * m_num_leds, m_pin);
    // Re-enable interrupts
    taskEXIT_CRITICAL();
    // WS2812 reset time:
    ets_delay_us(WS2812_RESET_US);
}

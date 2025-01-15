#ifndef __ADDR_LED_H__
#define __ADDR_LED_H__

#include <stdint.h>
#include <vector>

typedef union addrLedColor_t
{
    uint32_t color;
    struct
    {
        uint8_t b;
        uint8_t g;
        uint8_t r;
    };
} addrLedColor_t;


class AddrLed
{
    private:
        const uint32_t m_num_leds;
        const uint8_t m_pin;
        std::vector<addrLedColor_t> m_pixels;

    public:
        AddrLed(uint32_t num_leds, uint8_t pin);
        ~AddrLed();
        void init();
        void setPixelColor(uint32_t index, addrLedColor_t color);
        void fill(addrLedColor_t color);
        void fill(addrLedColor_t color, uint32_t start = 0, uint32_t end = 0);
        void show();
};

#endif // __ADDR_LED_H__

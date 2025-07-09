#ifndef __ADDR_LED_HPP__
#define __ADDR_LED_HPP__

#include <stdint.h>
#include <vector>

typedef union
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
        void fill(uint8_t r, uint8_t g, uint8_t b);
        void show();
};

#endif // __ADDR_LED_HPP__

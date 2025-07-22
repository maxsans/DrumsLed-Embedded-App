#ifndef __WS2812_ESP8266_HPP__
#define __WS2812_ESP8266_HPP__

#include <cstdint>
#include <vector>

union ws2812Esp8266Color_t
{
    struct
    {
        uint8_t r;
        uint8_t g;
        uint8_t b;
    };
    uint32_t color;
};

class WS2812Esp8266
{
    private:
        const uint32_t m_num_leds;
        const uint8_t m_pin;
        std::vector<ws2812Esp8266Color_t> m_pixels;

    public:
        WS2812Esp8266(uint32_t num_leds, uint8_t pin);
        void init();
        void setPixelColor(uint32_t index, uint8_t r, uint8_t g, uint8_t b);
        void fill(uint8_t r, uint8_t g, uint8_t b);
        void show();
};

#endif // __WS2812_ESP8266_HPP__

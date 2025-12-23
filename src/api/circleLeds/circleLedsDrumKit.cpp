/**
 * @file circleLedsDrumKit.cpp
 * @author Cyprien Ménard
 * @date 2025-08-20
 */

#include "api/drivers/ws2812Esp8266/ws2812Esp8266.hpp"
#include "circleLeds.hpp"
#include <stdio.h>

/**
 * @brief GPIO pin used for the circle LEDs.
 */
#define CIRCLE_LEDS_PIN 4

/**
 * @brief Number of LEDs in the circle.
 * @todo This value should be written in the nvs partition and read from there.
 *       This will allow to change the number of LEDs without recompiling the code.
 */
#define CIRCLE_LEDS_COUNT 200

/**
 * @brief The WS2812 driver instance for the circle LEDs.
 */
static WS2812Esp8266 g_ledDriver(CIRCLE_LEDS_COUNT, CIRCLE_LEDS_PIN);

void circleLedsInit()
{
    g_ledDriver.init();
}

uint32_t circleLedsGetCount()
{
    return CIRCLE_LEDS_COUNT;
}

void circleLedsFill(uint32_t color)
{
    circleLedsFill((color >> 16) & 0xFF, (color >> 8) & 0xFF, color & 0xFF);
}

void circleLedsFill(uint8_t red, uint8_t green, uint8_t blue)
{
    g_ledDriver.fill(red, green, blue);
}

void circleLedsSetPixel(uint32_t index, uint32_t color)
{
    circleLedsSetPixel(
        index, (color >> 16) & 0xFF, (color >> 8) & 0xFF, color & 0xFF);
}

void circleLedsSetPixel(uint32_t index,
                        uint8_t red,
                        uint8_t green,
                        uint8_t blue)
{
    g_ledDriver.setPixelColor(index, red, green, blue);
}

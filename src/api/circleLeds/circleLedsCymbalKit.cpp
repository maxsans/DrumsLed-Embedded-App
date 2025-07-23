#include "circleLeds.hpp"
#include "api/drivers/ws2812Esp8266/ws2812Esp8266.hpp"

/**
 * @brief GPIO pin used for the circle LEDs (bottom one).
 */
#define CIRCLE_LEDS_PIN_BOT 4

/**
 * @brief GPIO pin used for the circle LEDs (top one).
 */
#define CIRCLE_LEDS_PIN_TOP 5

/**
 * @brief Number of LEDs in the circle.
 * @note The bottom and top parts of the circle should have the same number of LEDs.
 */
#define CIRCLE_LEDS_COUNT 16

/**
 * @brief The WS2812 driver instance for the circle LEDs.
 */
static WS2812Esp8266 g_ledDriverBot(CIRCLE_LEDS_COUNT, CIRCLE_LEDS_PIN_BOT);
static WS2812Esp8266 g_ledDriverTop(CIRCLE_LEDS_COUNT, CIRCLE_LEDS_PIN_TOP);

void circleLedsInit()
{
    g_ledDriverBot.init();
    g_ledDriverTop.init();
}

uint32_t circleLedsGetCount()
{
    return CIRCLE_LEDS_COUNT;
}

void circleLedsFill(uint32_t color)
{
    circleLedsFill((color >> 16) & 0xFF, (color >> 8) & 0xFF, color & 0xFF);
}

void circleLedsFill(uint8_t red, uint8_t green, uint8_t blue) {
    g_ledDriverBot.fill(red, green, blue);
    g_ledDriverTop.fill(red, green, blue);
}

void circleLedsSetPixel(uint32_t index, uint32_t color)
{
    circleLedsSetPixel(index, (color >> 16) & 0xFF, (color >> 8) & 0xFF, color & 0xFF);
}

void circleLedsSetPixel(uint32_t index, uint8_t red, uint8_t green, uint8_t blue)
{
    g_ledDriverBot.setPixelColor(index, red, green, blue);
    g_ledDriverTop.setPixelColor(index, red, green, blue);
}

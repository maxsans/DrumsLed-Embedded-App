#ifndef __CIRCLE_LEDS_HPP__
#define __CIRCLE_LEDS_HPP__


#include <cstdint>

/**
 * @brief Initialize the circle LEDs.
 */
void circleLedsInit();

/**
 * @brief Get the number of leds in the circle.
 * @return The number of leds in the circle.
 */
uint32_t circleLedsGetCount();

/**
 * @brief Fill the circle LEDs with a specific color.
 * @param color The color to fill the LEDs with, in RGB format (0xRRGGBB).
 */
void circleLedsFill(uint32_t color);

/**
 * @brief Fill the circle LEDs with a specific color.
 * @param red The red component of the color (0-255).
 * @param green The green component of the color (0-255).
 * @param blue The blue component of the color (0-255).
 */
void circleLedsFill(uint8_t red, uint8_t green, uint8_t blue);

/**
 * @brief Set the color of a specific pixel in the circle LEDs.
 * @param index The index of the pixel to set (0-based).
 * @param color The color to set the pixel to, in RGB format (0xRRGGBB).
 */
void circleLedsSetPixel(uint32_t index, uint32_t color);

/**
 * @brief Set the color of a specific pixel in the circle LEDs.
 * @param index The index of the pixel to set (0-based).
 * @param red The red component of the color (0-255).
 * @param green The green component of the color (0-255).
 * @param blue The blue component of the color (0-255).
 */
void circleLedsSetPixel(uint32_t index, uint8_t red, uint8_t green, uint8_t blue);

#endif // __CIRCLE_LEDS_HPP__

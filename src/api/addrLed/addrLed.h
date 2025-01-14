#ifndef __ADDR_LED_H__
#define __ADDR_LED_H__

#include <stdint.h>

/**
 * @brief Type for the addressable LED color
 */
typedef uint32_t addrLedColor_t;

/**
 * @brief Type for the addressable LED index
 */
typedef uint32_t addrLedIndex_t;

/**
 * @brief Initialize the addressable LED
 *
 * @param num_leds Number of LEDs in the strip
 */
void addrLed_init(int num_leds);

/**
 * @brief Set the color of an addressable LED
 *
 * @param index Index of the LED
 * @param color Color of the LED
 */
void addrLed_set_color(addrLedIndex_t index, addrLedColor_t color);

/**
 * @brief Refresh the addressable LED strip
 */
void addrLed_refresh();

#endif // __ADDR_LED_H__

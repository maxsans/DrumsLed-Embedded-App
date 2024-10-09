#ifndef __RGB_LED_H__
#define __RGB_LED_H__

#include "colorOrder.h"
#include "module.h"

#include <stdint.h>

/**
 * @brief Enumerates the color priorities.
 * @warning The order of the elements is important.
 * @note Lower values have higher priority.
 *
 */
typedef enum
{
    // RESERVED
    COLOR_PRIORITY_LEARNING,

    // ANIMATIONS FROM HERE
    COLOR_PRIORITY_FADE,
    COLOR_PRIORITY_SIMPLE_COLOR,
    // TO HERE

    // END OF ENUM, DON'T ADD ELEMENTS AFTER THIS
    COLOR_PRIORITY_COUNT,
} colorPriority_t;


/**
 * @brief Class to represent an RGB LED.
 */
class rgbLed
{
    private:
        colorOrder m_colorOrders[COLOR_PRIORITY_COUNT];
        module *m_module;
        uint8_t m_gamma8[256];
        colorPriority_t m_priority;
        uint64_t m_lastSetTime;
        bool m_permColorSet;

    public:
        rgbLed(module *m);
        /**
         * @brief Get the Module object
         *
         * @return module* The module.
         */
        module *getModule();
        /**
         * @brief Set the color.
         *
         * @param color The color.
         */
        rgbColor getColor();
        /**
         * @brief Set the color.
         *
         * @param color The color.
         */
        void setColor(colorPriority_t priority, rgbColor color);
        /**
         * @brief Release the color.
         * @warning Don't forget to release the color when you don't need it anymore.
         */
        void releaseColor(colorPriority_t priority);
};

#endif

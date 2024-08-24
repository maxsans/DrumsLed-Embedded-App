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
        module *getModule();
        rgbColor getColor();
        void setColor(colorPriority_t priority, rgbColor color);
        void releaseColor(colorPriority_t priority);
};

#endif

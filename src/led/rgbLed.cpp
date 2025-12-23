#include "rgbLed.hpp"

#include <assert.h>
#include <cmath>

RgbLed::RgbLed()
{
}

RgbColor RgbLed::getColor()
{
    // Sherch for the highest priority enabled color
    for (uint32_t i = 0; i < COLOR_PRIORITY_COUNT; i++)
    {
        if (m_colorOrders[i].isEnabled())
        {
            return m_colorOrders[i].getColor();
        }
    }
    // No color is enabled
    // Return black
    return RgbColor(0, 0, 0);
}

void RgbLed::setColor(colorPriority_t priority, RgbColor color)
{
    // Set the color
    assert(priority < COLOR_PRIORITY_COUNT);
    m_colorOrders[priority].setColor(color);
}

void RgbLed::releaseColor(colorPriority_t priority)
{
    // Release the color
    assert(priority < COLOR_PRIORITY_COUNT);
    m_colorOrders[priority].releaseColor();
}

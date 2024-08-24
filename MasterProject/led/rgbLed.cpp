#include "rgbLed.h"

#include <cmath>
#include <windows.h>
#include <assert.h>

#define GAMMA 2.314

rgbLed::rgbLed(module *m)
{
    // Constructor
    m_module = m;
    // Initialize gamma correction
    for (int i = 0; i < 256; i++)
    {
        m_gamma8[i] = (uint8_t)(pow((float)i / 255.0, GAMMA) * 255.0 + 0.5);
    }
}

module *rgbLed::getModule()
{
    return m_module;
}

rgbColor rgbLed::getColor()
{
    // Sherch for the highest priority enabled color
    for (int i = 0; i < COLOR_PRIORITY_COUNT; i++)
    {
        if (m_colorOrders[i].isEnabled())
        {
            return m_colorOrders[i].getColor();
        }
    }
    // No color is enabled
    // Return black
    return rgbColor(0, 0, 0);
}

void rgbLed::setColor(colorPriority_t priority, rgbColor color)
{
    // Apply gamma correction
    uint8_t redValue, greenValue, blueValue;
    color.getColor(&redValue, &greenValue, &blueValue);
    redValue = m_gamma8[redValue];
    greenValue = m_gamma8[greenValue];
    blueValue = m_gamma8[blueValue];
    color.setColor(redValue, greenValue, blueValue);

    // Set the color
    assert(priority < COLOR_PRIORITY_COUNT);
    m_colorOrders[priority].setColor(color);
}

void rgbLed::releaseColor(colorPriority_t priority)
{
    // Release the color
    assert(priority < COLOR_PRIORITY_COUNT);
    m_colorOrders[priority].releaseColor();
}

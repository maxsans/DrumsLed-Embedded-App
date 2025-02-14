#include "rgbLed.h"

#include <cmath>
#include <assert.h>

#define GAMMA 2.314

RgbLed::RgbLed(Module *m)
{
    // Constructor
    m_module = m;
    // Initialize gamma correction
    for (uint16_t i = 0; i < 256; i++)
    {
        m_gamma8[i] = (uint8_t)(pow((float)i / 255.0, GAMMA) * 255.0 + 0.5);
    }
}

Module *RgbLed::getModule()
{
    return m_module;
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

void RgbLed::releaseColor(colorPriority_t priority)
{
    // Release the color
    assert(priority < COLOR_PRIORITY_COUNT);
    m_colorOrders[priority].releaseColor();
}

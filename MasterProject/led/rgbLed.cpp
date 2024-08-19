#include "rgbLed.h"

#include <cmath>

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

void rgbLed::getColor(uint8_t *redValue, uint8_t *greenValue, uint8_t *blueValue)
{
    m_color.getColor(redValue, greenValue, blueValue);
}

void rgbLed::setColor(uint8_t redValue, uint8_t greenValue, uint8_t blueValue)
{
    // Apply gamma correction
    redValue = m_gamma8[redValue];
    greenValue = m_gamma8[greenValue];
    blueValue = m_gamma8[blueValue];

    // Set the color
    m_color.setColor(redValue, greenValue, blueValue);
}

void rgbLed::setColor(uint32_t color)
{
    m_color.setColor(color);
}

uint32_t rgbLed::getColor()
{
    return m_color.getColor();
}

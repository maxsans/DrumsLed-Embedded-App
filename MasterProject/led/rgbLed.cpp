#include "rgbLed.h"

rgbLed::rgbLed(module *m)
{
    // Constructor
    m_module = m;
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

#include "rgbColor.h"

rgbColor::rgbColor()
{
    m_redValue = 0;
    m_greenValue = 0;
    m_blueValue = 0;
}

void rgbColor::setColor(uint8_t redValue, uint8_t greenValue, uint8_t blueValue)
{
    m_redValue = redValue;
    m_greenValue = greenValue;
    m_blueValue = blueValue;
}

void rgbColor::setColor(uint32_t color)
{
    m_redValue = (color >> 16) & 0xFF;
    m_greenValue = (color >> 8) & 0xFF;
    m_blueValue = color & 0xFF;
}

void rgbColor::getColor(uint8_t *redValue, uint8_t *greenValue, uint8_t *blueValue)
{
    *redValue = m_redValue;
    *greenValue = m_greenValue;
    *blueValue = m_blueValue;
}

uint32_t rgbColor::getColor()
{
    return ((uint32_t)m_redValue << 16) | ((uint32_t)m_greenValue << 8) | m_blueValue;
}

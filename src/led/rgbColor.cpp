#include "rgbColor.hpp"

RgbColor::RgbColor()
{
    setColor(0, 0, 0);
}

RgbColor::RgbColor(uint8_t redValue, uint8_t greenValue, uint8_t blueValue)
{
    setColor(redValue, greenValue, blueValue);
}

RgbColor::RgbColor(RawColor raw)
{
    setColor(raw);
}

void RgbColor::setColor(uint8_t redValue, uint8_t greenValue, uint8_t blueValue)
{
    m_rawColor.r = redValue;
    m_rawColor.g = greenValue;
    m_rawColor.b = blueValue;
}

void RgbColor::setColor(RawColor raw)
{
    m_rawColor = raw;
}

void RgbColor::getColor(uint8_t *redValue,
                        uint8_t *greenValue,
                        uint8_t *blueValue)
{
    *redValue = m_rawColor.r;
    *greenValue = m_rawColor.g;
    *blueValue = m_rawColor.b;
}

RgbColor::RawColor RgbColor::getRaw()
{
    return m_rawColor;
}

uint8_t RgbColor::getRed()
{
    return m_rawColor.r;
}

uint8_t RgbColor::getGreen()
{
    return m_rawColor.g;
}

uint8_t RgbColor::getBlue()
{
    return m_rawColor.b;
}

RgbColor RgbColor::operator*(float value)
{
    return RgbColor(static_cast<uint8_t>(value * m_rawColor.r),
                    static_cast<uint8_t>(value * m_rawColor.g),
                    static_cast<uint8_t>(value * m_rawColor.b));
}

#include "colorOrder

ColorOrder::ColorOrder()
{
    // Constructor
    m_enable = false;
}

void ColorOrder::setColor(RgbColor color)
{
    m_color = color;
    m_enable = true;
}

RgbColor ColorOrder::getColor()
{
    return m_color;
}

bool ColorOrder::isEnabled()
{
    return m_enable;
}

void ColorOrder::releaseColor()
{
    m_enable = false;
}

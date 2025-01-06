#include "colorOrder.h"

colorOrder::colorOrder()
{
    // Constructor
    m_enable = false;
}

void colorOrder::setColor(rgbColor color)
{
    m_color = color;
    m_enable = true;
}

rgbColor colorOrder::getColor()
{
    return m_color;
}

bool colorOrder::isEnabled()
{
    return m_enable;
}

void colorOrder::releaseColor()
{
    m_enable = false;
}

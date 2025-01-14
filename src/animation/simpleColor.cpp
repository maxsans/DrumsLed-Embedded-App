#include "simpleColor.h"

simpleColor::simpleColor(rgbLed *rgbLed, rgbColor color) : animation(ANIMATION_TYPE_SIMPLE_COLOR, NULL, rgbLed)
{
    m_color = color;
}

simpleColor::~simpleColor()
{
}

void simpleColor::start()
{
    m_rgbLed->setColor(COLOR_PRIORITY_SIMPLE_COLOR, m_color);
}

void simpleColor::process()
{
}

void simpleColor::stop()
{
    m_rgbLed->releaseColor(COLOR_PRIORITY_SIMPLE_COLOR);
}

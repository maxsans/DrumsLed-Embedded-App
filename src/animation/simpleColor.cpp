#include "simpleColor

SimpleColor::SimpleColor(RgbLed *rgbLed, RgbColor color) : Animation(ANIMATION_TYPE_SIMPLE_COLOR, NULL, rgbLed)
{
    m_color = color;
}

SimpleColor::~SimpleColor()
{
}

void SimpleColor::start()
{
    m_rgbLed->setColor(COLOR_PRIORITY_SIMPLE_COLOR, m_color);
}

void SimpleColor::process()
{
}

void SimpleColor::stop()
{
    m_rgbLed->releaseColor(COLOR_PRIORITY_SIMPLE_COLOR);
}

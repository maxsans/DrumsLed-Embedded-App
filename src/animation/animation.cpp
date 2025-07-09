#include "animation

Animation::Animation(animationType_t type, Micro *m, RgbLed *rgbLed)
{
    m_type = type;
    m_micro = m;
    m_rgbLed = rgbLed;
}

Animation::~Animation()
{
}

animationType_t Animation::getType()
{
    return m_type;
}

Micro *Animation::getMicro()
{
    return m_micro;
}

RgbLed *Animation::getRgbLed()
{
    return m_rgbLed;
}

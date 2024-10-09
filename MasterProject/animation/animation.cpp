#include "animation.h"

animation::animation(animationType type, micro *m, rgbLed *rgbLed)
{
    m_type = type;
    m_micro = m;
    m_rgbLed = rgbLed;
}

animation::~animation()
{
}

animationType animation::getType()
{
    return m_type;
}

micro *animation::getMicro()
{
    return m_micro;
}

rgbLed *animation::getRgbLed()
{
    return m_rgbLed;
}

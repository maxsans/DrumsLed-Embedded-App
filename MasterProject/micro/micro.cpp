#include "micro.h"


micro::micro(module *m)
{
    // Constructor
    m_module = m;
    m_microValue = 0;
}

module *micro::getModule()
{
    // Get the module of the micro
    return m_module;
}

void micro::setMicroValue(uint8_t microValue)
{
    // Set the value of the micro
    m_microValue = microValue;
}

uint8_t micro::getMicroValue()
{
    // Get the value of the micro
    return m_microValue;
}

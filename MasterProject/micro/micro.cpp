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

void micro::setCorrection(coeff correction)
{
    // Set the correction of the micro
    m_correction = correction;
}

coeff micro::getCorrection()
{
    // Get the correction of the micro
    return m_correction;
}

void micro::setMicroValue(uint8_t microValue)
{
    // Set the value of the micro
    m_microValue = microValue;
    // Correct the value of the micro
    uint32_t l_microValueCorrected = m_microValue * m_correction.m_value / QUANTUM_COEFF;
    // Check if the value is not too high
    if (l_microValueCorrected > 255)
    {
        m_microValueCorrected = 255;
    }
    else
    {
        m_microValueCorrected = l_microValueCorrected;
    }
}

uint8_t micro::getMicroValue()
{
    // Get the value of the micro
    return m_microValue;
}

uint8_t micro::getMicroValueCorrected()
{
    // Get the corrected value of the micro
    return m_microValueCorrected;
}

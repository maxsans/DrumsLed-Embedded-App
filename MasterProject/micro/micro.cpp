#include "micro.h"


micro::micro(module *m)
{
    // Constructor
    m_module = m;
    m_microValue = 0;
    m_microValueCorrected = 0;
    m_correction.m_value = 1;
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
}

void micro::setMicroValueCorrected(uint8_t microValueCorrected)
{
    // Set the corrected value of the micro
    m_microValueCorrected = microValueCorrected;
}

void micro::setThreshold(uint8_t threshold)
{
    // Set the threshold of the micro
    m_threshold = threshold;
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

uint8_t micro::getThreshold()
{
    // Get the threshold of the micro
    return m_threshold;
}

bool micro::isHit()
{
    // Check if the micro is hit
    return (m_microValueCorrected > (m_threshold*m_correction.m_value/QUANTUM_COEFF));
}

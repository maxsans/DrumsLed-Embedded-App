#include "coeff.h"

coeff::coeff()
{
    m_value = 0;
}

coeff::coeff(float value)
{
    setFloat(value);
}

void coeff::setFloat(float coeff)
{
    m_value = coeff * QUANTUM_COEFF;
}

float coeff::toFloat()
{
    return (float)m_value / QUANTUM_COEFF;
}

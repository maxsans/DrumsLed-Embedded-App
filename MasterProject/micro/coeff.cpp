#include "coeff.h"

coeff::coeff()
{
    m_value = 0;
}

void coeff::setFloat(float coeff)
{
    m_value = coeff * QUANTUM_COEFF;
}

float coeff::toFloat()
{
    return (float)m_value / QUANTUM_COEFF;
}

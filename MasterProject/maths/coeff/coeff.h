#ifndef __COEFF_H__
#define __COEFF_H__

#include <stdint.h>

#define QUANTUM_COEFF 1000

/**
 * @brief Coeffs are integers that represent the sound impact of an instrument on an other instrument.
 * @warning QUANTUM_COEFF is the smallest unit of impact that can be represented.
 * so 1 in reality is QUANTUM_COEFF in the impact unit.
 * and 1 in the impact unit is 1/QUANTUM_COEFF in reality.
 *
 */
class coeff
{
    public:
        coeff();
        coeff(float value);
        int64_t m_value;
        void setFloat(float coeff);
        float toFloat();
};

#endif
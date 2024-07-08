#ifndef __MATRIX_H__
#define __MATRIX_H__

#include "coeff.h"

#include <stdint.h>

class matrix
{
    private:
        uint32_t m_size;
        coeff **m_data;

    public:
        matrix(uint32_t size);
        ~matrix();
        void print();
        void set(uint32_t x, uint32_t y, coeff value);
        coeff get(uint32_t x, uint32_t y);
        void invert();
};

#endif

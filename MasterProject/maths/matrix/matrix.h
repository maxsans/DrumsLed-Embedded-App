#ifndef __MATRIX_H__
#define __MATRIX_H__


#include <stdint.h>

class matrix
{
    private:
        uint32_t m_size;
        float **m_data;

    public:
        matrix(uint32_t size);
        ~matrix();
        void print();
        void set(uint32_t x, uint32_t y, float value);
        float get(uint32_t x, uint32_t y);
        void invert();
};

#endif

#ifndef __MATRIX_H__
#define __MATRIX_H__


#include <stdint.h>

/**
 * @brief Generic class to represent a matrix and perform operations on it.
 *
 */
class matrix
{
    private:
        uint32_t m_size;
        float **m_data;

    public:
        matrix(uint32_t size);
        ~matrix();
        /**
         * @brief Function to print the matrix in the console.
         */
        void print();
        /**
         * @brief Function to set the value of a cell.
         *
         * @param x The x coordinate.
         * @param y The y coordinate.
         * @param value The value to set.
         */
        void set(uint32_t x, uint32_t y, float value);
        /**
         * @brief Function to get the value of a cell.
         *
         * @param x The x coordinate.
         * @param y The y coordinate.
         * @return float The value.
         */
        float get(uint32_t x, uint32_t y);
        /**
         * @brief Function to invert the matrix.
         */
        void invert();
};

#endif

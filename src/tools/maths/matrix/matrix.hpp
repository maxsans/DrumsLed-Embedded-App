/**
 * @file matrix.hpp
 * @brief Square matrix mathematical operations including inversion
 * @author Cyprien Ménard
 * @date 2025-08-20
 */

#ifndef __MATRIX_HPP__
#define __MATRIX_HPP__

#include <stdint.h>

/**
 * @brief Square matrix class with mathematical operations including Gauss-Jordan inversion
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

#endif // __MATRIX_HPP__

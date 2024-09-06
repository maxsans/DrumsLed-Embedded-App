#include "matrix.h"

#include <stdio.h>
#include <assert.h>

matrix::matrix(uint32_t size)
{
    m_size = size;
    m_data = new float*[size];
    for (uint32_t i = 0; i < size; i++)
    {
        m_data[i] = new float[size];
    }
    // Fill the matrix with 0
    for (uint32_t i = 0; i < size; i++)
    {
        for (uint32_t j = 0; j < size; j++)
        {
            m_data[i][j] = 0;
        }
    }
}

matrix::~matrix()
{
    for (uint32_t i = 0; i < m_size; i++)
    {
        delete[] m_data[i];
    }
    delete[] m_data;
}

void matrix::print()
{
    printf("Matrix of size %d\n", m_size);
    for (uint32_t i = 0; i < m_size; i++)
    {
        for (uint32_t j = 0; j < m_size; j++)
        {
            printf("%.2f   ", m_data[i][j]);
        }
        printf("\n");
    }
}

void matrix::set(uint32_t x, uint32_t y, float value)
{
    assert(x < m_size && y < m_size && x >= 0 && y >= 0);
    m_data[x][y] = value;
}

float matrix::get(uint32_t x, uint32_t y)
{
    assert(x < m_size && y < m_size && x >= 0 && y >= 0);
    return m_data[x][y];
}

void matrix::invert()
{
    // Invert the matrix
    // First create a id juxtaposed matrix
    // Then apply the Gauss-Jordan elimination method
    // to the matrix
    // Finally, the matrix will be inverted

    // Create a matrix with the size of m_size x 2m_size
    float **l_juxMatrix = new float*[m_size];
    for (uint32_t i = 0; i < m_size; i++)
    {
        l_juxMatrix[i] = new float[2 * m_size];
    }
    // Fill the left side of the matrix with the original matrix
    for (uint32_t i = 0; i < m_size; i++)
    {
        for (uint32_t j = 0; j < m_size; j++)
        {
            l_juxMatrix[i][j] = m_data[i][j];
        }
    }
    // Fill the right side of the matrix with the identity matrix
    for (uint32_t i = 0; i < m_size; i++)
    {
        for (uint32_t j = m_size; j < 2 * m_size; j++)
        {
            if (i == j - m_size)
            {
                l_juxMatrix[i][j] = 1;
            }
            else
            {
                l_juxMatrix[i][j] = 0;
            }
        }
    }

    // Apply the Gauss-Jordan elimination method
    for(uint32_t l_column=0 ; l_column<m_size ; l_column++)
    {
        for (uint32_t l_line=0;l_line<m_size;l_line++)
        {
            if (l_line != l_column)
            {
                // Constant to eliminate
                float l_elimConst = l_juxMatrix[l_line][l_column];
                // Check if the constant is not null
                // If it is null, it is useless to eliminate
                if (l_elimConst != 0)
                {
                    // Eliminate the constant
                    uint32_t l_elimLine = l_column;
                    for (uint32_t l_column2=0;l_column2<2*m_size;l_column2++)
                    {
                        l_juxMatrix[l_line][l_column2] -= l_juxMatrix[l_elimLine][l_column2] * l_elimConst;
                    }
                    // Then we make again the 1 on the diagonal
                    float l_diagConst = l_juxMatrix[l_line][l_line];
                    for (uint32_t l_column2=0;l_column2<2*m_size;l_column2++)
                    {
                        l_juxMatrix[l_line][l_column2] = l_juxMatrix[l_line][l_column2] / l_diagConst;
                    }
                }
            }
        }
    }

    // Fill the original matrix with the inverted matrix
    for (uint32_t i = 0; i < m_size; i++)
    {
        for (uint32_t j = 0; j < m_size; j++)
        {
            m_data[i][j] = l_juxMatrix[i][j + m_size];
        }
    }
}

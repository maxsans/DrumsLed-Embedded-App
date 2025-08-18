/**
 * @file program.hpp
 * @brief Program api for slave targets.
 * @author Cyprien Ménard
 * @date 2025-08-18
 */

#ifndef __PROGRAM_HPP__
#define __PROGRAM_HPP__

#include "tools/containers/binary/binary.hpp"

/**
 * @brief Program API for slave targets.
 */
class Program
{
    private:
    public:
    /**
     * @brief Get the binary of the program.
     * @param [out] binary Binary object to fill with the program data.
     */
    static void getProgramBinary(Binary *binary);

    /**
     * @brief Update the program of the slave target.
     * @param binary Binary object containing the new program data.
     * @warning This function will erase the current program.
     * @note A reset will be triggered after the update.
     */
    static void updateProgramBinary(const Binary &binary);
};

#endif // __PROGRAM_HPP__

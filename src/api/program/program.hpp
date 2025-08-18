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
    public:
    /**
     * @brief Get the binary of the program.
     * @param [out] binary Binary object to fill with the program data.
     */
    static void getProgramBinary(Binary *binary);

    /**
     * @brief Begin program update process.
     * @param total_size Total size of the binary that will be written.
     * @return true if successfully initialized, false otherwise.
     */
    static bool beginProgramUpdate(size_t total_size);

    /**
     * @brief Update the program of the slave target with a chunk of data.
     * @param binary Binary chunk containing part of the new program data.
     * @return true if chunk was written successfully, false otherwise.
     * @note Call beginProgramUpdate() first, then multiple calls to this function.
     */
    static bool updateProgramBinary(const Binary &binary);

    /**
     * @brief Finalize the program update and restart the system.
     * @return true if finalization was successful, false otherwise.
     * @note This function will trigger a system restart if successful.
     */
    static bool finalizeProgramUpdate();
};

#endif // __PROGRAM_HPP__

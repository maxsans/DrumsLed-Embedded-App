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
    /**
     * @brief Total size of the program binary.
     */
    static size_t m_totalSize;

    /**
     * @brief Current size of the program binary.
     */
    static size_t m_currentSize;

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

    /**
     * @brief Validate the current running firmware after OTA update.
     * @return true if validation is successful, false otherwise.
     * @note This should be called after restart to confirm the new firmware works.
     */
    static bool validateCurrentFirmware();

    /**
     * @brief Rollback to previous firmware partition if current one is invalid.
     * @return true if rollback was successful, false otherwise.
     */
    static bool rollbackToPreviousFirmware();

    /**
     * @brief Initialize automatic rollback protection.
     * @param max_boot_attempts Maximum boot attempts before automatic rollback.
     * @note This should be called early in main() before any other initialization.
     *       Validation can be done manually at any time with validateCurrentFirmware().
     */
    static void initAutomaticRollback(uint8_t max_boot_attempts = 3);

    /**
     * @brief Check and handle automatic rollback if needed.
     * @note Called internally by initAutomaticRollback(), but can be called manually.
     */
    static void checkAutomaticRollback();

    /**
     * @brief Get current boot attempt count.
     * @return Current boot attempt number, 0 if not in OTA state.
     */
    static uint8_t getCurrentBootAttempt();

    /**
     * @brief Check if the total size of the program binary has been reached.
     * @return true if total size is reached, false otherwise.
     */
    static bool isTotalSizeReached();
};

#endif // __PROGRAM_HPP__

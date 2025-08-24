/**
 * @file crc32.hpp
 * @brief CRC32 utility functions
 * @author Cyprien Ménard
 * @date 2025-08-19
 */

#ifndef __CRC32_HPP__
#define __CRC32_HPP__

#include <cstddef>
#include <cstdint>

/**
 * @brief CRC32 utility class for checksum calculations
 */
class CRC32
{
    private:
    static const uint32_t CRC_TABLE[256];

    public:
    /**
     * @brief Calculate CRC32 for a data buffer
     * @param data Pointer to data buffer
     * @param size Size of data buffer
     * @return CRC32 checksum
     */
    static uint32_t calculate(const uint8_t *data, size_t size);

    /**
     * @brief Calculate CRC32 incrementally
     * @param crc Current CRC value (use 0xFFFFFFFF for initial value)
     * @param data Pointer to data buffer
     * @param size Size of data buffer
     * @return Updated CRC32 checksum
     */
    static uint32_t update(uint32_t crc, const uint8_t *data, size_t size);

    /**
     * @brief Finalize CRC32 calculation
     * @param crc Current CRC value
     * @return Final CRC32 checksum
     */
    static uint32_t finalize(uint32_t crc);
};

#endif // __CRC32_HPP__

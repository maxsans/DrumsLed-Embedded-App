/**
 * @file binary.hpp
 * @brief Binary container
 * @author Cyprien Ménard
 * @date 2025-08-18
 */

#ifndef __BINARY_HPP__
#define __BINARY_HPP__

#include <cstddef>
#include <cstdint>
#include <vector>

/**
 * @brief Class representing a binary container.
 */
class Binary
{
    private:
    /**
     * @brief Internal data storage for the binary container.
     */
    std::vector<uint8_t> m_data;

    public:
    /**
     * @brief Create an empty binary container.
     */
    Binary();

    /**
     * @brief Create a binary container from a byte array.
     * @param bytes Pointer to the byte array.
     * @param size Size of the byte array.
     */
    Binary(const uint8_t *bytes, size_t size);

    /**
     * @brief Create a binary container from a vector of bytes.
     * @param bytes Vector containing the byte data.
     */
    Binary(const std::vector<uint8_t> &bytes);

    /**
     * @brief Get the size of the binary container.
     * @return Size in bytes.
     */
    size_t size() const;

    /**
     * @brief Allocate memory for the binary container.
     * @param size Size in bytes to allocate.
     * @note If the current size is larger than the requested size, the allocation will not change.
     */
    void alloc(size_t size);

    /**
     * @brief Divide the binary container into consecutive chunks.
     * @param chunk_size Size of each chunk.
     * @return Vector of binary containers representing the chunks.
     */
    std::vector<Binary> chunk(size_t chunk_size) const;

    /**
     * @brief Calculate the CRC32 checksum of the binary container.
     * @return CRC32 checksum.
     */
    uint32_t crc32() const;

    /**
     * @brief Add a byte to the binary container.
     * @param byte Byte to add.
     */
    Binary &operator+=(uint8_t byte);

    /**
     * @brief Add a binary container to the current one.
     * @param chunk Binary container to add.
     */
    Binary &operator+=(const Binary &chunk);

    /**
     * @brief Get a pointer to the underlying data.
     * @return Pointer to the data buffer.
     */
    uint8_t *data();

    /**
     * @brief Get a const pointer to the underlying data.
     * @return Const pointer to the data buffer.
     */
    const uint8_t *data() const;
};

#endif // __BINARY_HPP__

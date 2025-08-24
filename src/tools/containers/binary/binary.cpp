/**
 * @file binary.cpp
 * @author Cyprien Ménard
 * @date 2025-08-18
 */

#include "binary.hpp"
#include "crc32.hpp"

Binary::Binary() : m_data()
{
}

Binary::Binary(const uint8_t *bytes, size_t size) : m_data(bytes, bytes + size)
{
}

Binary::Binary(const std::vector<uint8_t> &bytes) : m_data(bytes)
{
}

size_t Binary::size() const
{
    return m_data.size();
}

void Binary::alloc(size_t size)
{
    if (size > m_data.size())
    {
        m_data.resize(size);
    }
}

std::vector<Binary> Binary::chunk(size_t chunk_size) const
{
    std::vector<Binary> chunks;
    size_t totalSize = m_data.size();

    for (size_t i = 0; i < totalSize; i += chunk_size)
    {
        size_t currentChunkSize = std::min(chunk_size, totalSize - i);
        chunks.emplace_back(m_data.data() + i, currentChunkSize);
    }

    return chunks;
}

uint32_t Binary::crc32() const
{
    return CRC32::calculate(m_data.data(), m_data.size());
}

Binary &Binary::operator+=(uint8_t byte)
{
    m_data.push_back(byte);
    return *this;
}

Binary &Binary::operator+=(const Binary &chunk)
{
    m_data.insert(m_data.end(), chunk.m_data.begin(), chunk.m_data.end());
    return *this;
}

uint8_t *Binary::data()
{
    return m_data.data();
}

const uint8_t *Binary::data() const
{
    return m_data.data();
}

#include "interMsgData.hpp"

#include <cstring>
#include <cassert>

InterMsgData::InterMsgData(InterMsgHeader header, char *privData)
    : m_header(header), m_privData(privData)
{
}

InterMsgData::InterMsgData(char *rawData, uint32_t rawSize, char *privData)
    : m_header(rawData), m_privData(privData)
{
    assert(rawData != nullptr && "Raw data pointer cannot be null.");
    // Check if the raw data size is sufficient
    if (rawSize >= m_header.getHeaderSize() + m_header.getPrivSize())
    {
        // Find the header size and private data size
        uint32_t headerSize = m_header.getHeaderSize();
        uint32_t privSize = m_header.getPrivSize();
        // Ensure the private data size does not exceed the maximum allowed size
        if (privSize <= MAX_PRIV_DATA_SIZE)
        {
            // Copy the private data from the raw data buffer
            std::memcpy(privData, rawData + headerSize, privSize);
        }
    }
}

uint32_t InterMsgData::getPrivData(char data[MAX_PRIV_DATA_SIZE]) const
{
    assert(data != nullptr && "Data pointer cannot be null.");
    assert(m_header.getPrivSize() <= MAX_PRIV_DATA_SIZE && "Private data size exceeds maximum allowed size.");
    std::memcpy(data, m_privData, m_header.getPrivSize());
    return m_header.getPrivSize();
}

const InterMsgHeader& InterMsgData::getHeader() const
{
    return m_header;
}

bool InterMsgData::operator==(const InterMsgData &other) const
{
    if (m_header != other.m_header)
    {
        return false;
    }

    if (m_header.getPrivSize() == 0 && other.m_header.getPrivSize() == 0)
    {
        return true; // Both have no private data, considered equal
    }

    return std::memcmp(m_privData, other.m_privData, m_header.getPrivSize()) == 0;
}

bool InterMsgData::operator!=(const InterMsgData &other) const
{
    return !(*this == other);
}

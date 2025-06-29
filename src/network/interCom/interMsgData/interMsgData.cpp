#include "interMsgData.hpp"

#include <cstring>
#include <cassert>

InterMsgData::InterMsgData(InterMsgHeader header, char *privData)
    : m_header(header), m_privData(privData)
{
}

InterMsgData::InterMsgData(char *rawData, size_t rawSize)
    : m_header(rawData)
{
    assert(rawData != nullptr && "Raw data pointer cannot be null.");
    assert(rawSize >= m_header.getHeaderSize() + m_header.getPrivSize() && "Raw data size is insufficient for header and private data.");
    // Find the header size and private data size
    size_t headerSize = m_header.getHeaderSize();
    size_t privSize = m_header.getPrivSize();
    assert(privSize <= MAX_PRIV_DATA_SIZE && "Private data size exceeds maximum allowed size.");
    // Copy the private data from the raw data buffer
    m_privData = new char[privSize];
    std::memcpy(m_privData, rawData + headerSize, privSize);
    m_mustBeDeleted = true; // Indicate that the private data must be deleted
}

InterMsgData::~InterMsgData()
{
    if (m_mustBeDeleted && m_privData != nullptr)
    {
        delete[] m_privData; // Delete the private data if it was allocated
        m_privData = nullptr; // Set pointer to null to avoid dangling pointer
    }
}

size_t InterMsgData::getPrivData(char data[MAX_PRIV_DATA_SIZE]) const
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

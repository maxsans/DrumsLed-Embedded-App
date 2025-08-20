/**
 * @file interMsgHeader.cpp
 * @author Cyprien Ménard
 * @date 2025-08-20
 */

#include "interMsgHeader.hpp"

#include <cassert>
#include <cstring>

InterMsgHeader::InterMsgHeader(InterMsgId id, uint32_t privSize)
    : m_headerUnion{
          .m_header = {.m_id = id.rawValue(), .m_privSize = privSize}
}
{
    assert(privSize <= MAX_PRIV_DATA_SIZE
           && "Private data size exceeds maximum allowed size.");
}

InterMsgHeader::InterMsgHeader(const char *rawData) : m_headerUnion{}
{
    assert(rawData != nullptr && "Raw data pointer cannot be null.");
    std::memcpy(&m_headerUnion.m_header, rawData, getHeaderSize());
    assert(m_headerUnion.m_header.m_privSize <= MAX_PRIV_DATA_SIZE
           && "Private data size exceeds maximum allowed size.");
}

InterMsgId InterMsgHeader::getId() const
{
    return InterMsgId(m_headerUnion.m_header.m_id);
}

uint32_t InterMsgHeader::getPrivSize() const
{
    return m_headerUnion.m_header.m_privSize;
}

bool InterMsgHeader::operator==(const InterMsgHeader &other) const
{
    return (m_headerUnion.m_header.m_id == other.m_headerUnion.m_header.m_id
            && m_headerUnion.m_header.m_privSize
                   == other.m_headerUnion.m_header.m_privSize);
}

bool InterMsgHeader::operator!=(const InterMsgHeader &other) const
{
    return !(*this == other);
}

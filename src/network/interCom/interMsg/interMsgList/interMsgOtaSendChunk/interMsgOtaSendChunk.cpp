/**
 * @file interMsgOtaSendChunk.cpp
 * @author Cyprien Ménard
 * @date 2025-08-20
 */

#include "interMsgOtaSendChunk.hpp"
#include <cstring>

InterMsgOtaSendChunk::InterMsgOtaSendChunk(Client client,
                                           const char *chunkData,
                                           uint32_t chunkSize)
    : InterMsg(client, InterMsgId::OtaSendChunk, m_sendType)
{
    std::memcpy(m_chunkData, chunkData, chunkSize);
    m_chunkSize = chunkSize;
}

InterMsgOtaSendChunk::InterMsgOtaSendChunk(MessageReceivedCallback callback)
    : InterMsg(callback, InterMsgId::OtaSendChunk)
{
}

uint32_t InterMsgOtaSendChunk::serializePriv(char *data) const
{
    // Serialize the chunk data and size using network byte order
    std::memcpy(data, m_chunkData, m_chunkSize);
    std::memcpy(data + m_chunkSize, &m_chunkSize, sizeof(m_chunkSize));
    return m_chunkSize + sizeof(m_chunkSize);
}

uint32_t InterMsgOtaSendChunk::deserializePriv(const char *data)
{
    // Deserialize the chunk data and size using network byte order
    std::memcpy(m_chunkData, data, m_chunkSize);
    std::memcpy(&m_chunkSize, data + m_chunkSize, sizeof(m_chunkSize));
    return m_chunkSize + sizeof(m_chunkSize);
}

const char *InterMsgOtaSendChunk::getChunkData() const
{
    return m_chunkData;
}

uint32_t InterMsgOtaSendChunk::getChunkSize() const
{
    return m_chunkSize;
}

std::string InterMsgOtaSendChunk::toString() const
{
    return "InterMsgOtaSendChunk: client = "
           + getClient().getIP().getIpString();
}

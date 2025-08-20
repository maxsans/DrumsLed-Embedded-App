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
    : InterMsg(
          client,
          InterMsgData(InterMsgHeader(InterMsgId::OtaSendChunk, sizeof(m_data)),
                       (char *)&m_data),
          m_sendType)
{
    std::memcpy(m_data.m_chunkData, chunkData, chunkSize);
    m_data.m_chunkSize = chunkSize;
}

InterMsgOtaSendChunk::InterMsgOtaSendChunk(Client client,
                                           char *rawData,
                                           uint32_t size)
    : InterMsg(client, InterMsgData(rawData, size, (char *)&m_data), m_sendType)
{
    if (size == sizeof(m_data))
    {
        std::memcpy(&m_data, rawData, sizeof(m_data));
    }
}

const char *InterMsgOtaSendChunk::getChunkData() const
{
    return m_data.m_chunkData;
}

std::string InterMsgOtaSendChunk::toString() const
{
    return "InterMsgOtaSendChunk: client = "
           + getClient().getIP().getIpString();
}

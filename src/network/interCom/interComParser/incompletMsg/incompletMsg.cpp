/**
 * @file incompletMsg.cpp
 * @author Cyprien Ménard
 * @date 2025-08-18
 */

#include "incompletMsg.hpp"
#include "network/interCom/interMsgData/interMsgHeader/interMsgHeader.hpp"

IncompletMsg::IncompletMsg(const Client &client, const char *data, int16_t len)
    : m_client(client)
{
    if (data != nullptr && len > 0)
    {
        m_buffer.insert(m_buffer.end(), data, data + len);
    }
}

void IncompletMsg::appendData(const char *data, size_t len)
{
    if (data != nullptr && len > 0)
    {
        m_buffer.insert(m_buffer.end(), data, data + len);
    }
}

size_t IncompletMsg::getSize() const
{
    return m_buffer.size();
}

const char *IncompletMsg::getData() const
{
    return m_buffer.data();
}

const Client &IncompletMsg::getClient() const
{
    return m_client;
}

bool IncompletMsg::isComplete() const
{
    // To check if the message is complete, we need to check :
    // 1. If the size of the buffer is greater than or equal to the header size
    // 2. If the size of the private data is the same as the expected size in the header

    // Check if the buffer has enough data for the header
    if (m_buffer.size() < InterMsgHeader::getHeaderSize())
    {
        return false;
    }

    // Create a header from the buffer
    InterMsgHeader header(m_buffer.data());
    // Get the expected size from the header
    uint32_t expectedSize
        = InterMsgHeader::getHeaderSize() + header.getPrivSize();
    // Check if the buffer size is at least the expected size
    return expectedSize == m_buffer.size();
}

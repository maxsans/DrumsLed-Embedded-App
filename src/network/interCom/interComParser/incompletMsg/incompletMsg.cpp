/**
 * @file incompletMsg.cpp
 * @author Cyprien Ménard
 * @date 2025-08-18
 */

#include "incompletMsg.hpp"
#include "network/interCom/interMsg/interMsg.hpp"

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

void IncompletMsg::replaceData(const char *data, size_t len)
{
    m_buffer.clear();
    if (data != nullptr && len > 0)
    {
        m_buffer.insert(m_buffer.end(), data, data + len);
    }
}

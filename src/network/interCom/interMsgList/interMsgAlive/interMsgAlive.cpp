#include "interMsgAlive.hpp"

#include "tools/logStream/logStream.hpp"

constexpr const char *InterMsgAlive::m_data;

InterMsgAlive::InterMsgAlive(Client client)
    : InterMsg(client,
               InterMsgData(InterMsgHeader(InterMsgId::Alive, sizeof(m_data)),
                            (char *)&m_data),
               m_sendType)
{
}

InterMsgAlive::InterMsgAlive(Client client, char *rawData, uint32_t size)
    : InterMsg(client, InterMsgData(rawData, size, (char *)&m_data), m_sendType)
{
}

const char *InterMsgAlive::getPrivData() const
{
    return m_data;
}

std::string InterMsgAlive::toString() const
{
    return "InterMsgAlive: client = " + getClient().getIP().getIpString();
}

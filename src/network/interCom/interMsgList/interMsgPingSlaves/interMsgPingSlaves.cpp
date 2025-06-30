#include "interMsgPingSlaves.hpp"

#include "tools/logStream/logStream.h"

InterMsgPingSlaves::InterMsgPingSlaves(Client client)
    : InterMsg(client, InterMsgData(InterMsgHeader(InterMsgId::PingSlaves, sizeof(m_data)), (char *)&m_data), m_sendType)
{
}

InterMsgPingSlaves::InterMsgPingSlaves()
    : InterMsg(Client(), InterMsgData(InterMsgHeader(InterMsgId::PingSlaves, sizeof(m_data)), (char *)&m_data), m_sendType)
{
}

InterMsgPingSlaves::InterMsgPingSlaves(Client client, char *rawData, uint32_t size)
    : InterMsg(client, InterMsgData(rawData, size, (char *)&m_data), m_sendType)
{
}

const char* InterMsgPingSlaves::getPrivData() const
{
    return m_data;
}

std::string InterMsgPingSlaves::toString() const
{
    return "InterMsgPingSlaves: client = " + getClient().getIP().getIpString();
}

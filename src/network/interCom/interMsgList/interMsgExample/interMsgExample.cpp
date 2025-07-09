#include "interMsgExample.hpp"

#include "tools/logStream/logStream.h"

InterMsgExample::InterMsgExample(Client client, int exampleData)
    : InterMsg(client, InterMsgData(InterMsgHeader(InterMsgId::Example, sizeof(m_data)), (char *)&m_data), m_sendType)
{
    m_data.exampleData = exampleData;
}

InterMsgExample::InterMsgExample(Client client, char *rawData, uint32_t size)
    : InterMsg(client, InterMsgData(rawData, size, (char *)&m_data), m_sendType)
{
}

int InterMsgExample::getExampleData() const
{
    return m_data.exampleData;
}

std::string InterMsgExample::toString() const
{
    return "InterMsgExample: exampleData = " + std::to_string(m_data.exampleData)
           + ", client = " + getClient().getIP().getIpString();
}

#include "interMsgExample.hpp"

#include "tools/logStream/logStream.h"

InterMsgExample::Callback InterMsgExample::m_callback = nullptr;

InterMsgExample::InterMsgExample(Client client, int exampleData)
    : InterMsg(client, InterMsgData(InterMsgHeader(InterMsgId::Example, sizeof(m_data)), (char *)&m_data), m_sendType)
{
    m_data.exampleData = exampleData;
}

InterMsgExample::InterMsgExample(Client client, char *rawData, size_t size)
    : InterMsg(client, InterMsgData(rawData, size), m_sendType)
{
}

void InterMsgExample::registerCallback(Callback callback)
{
    m_callback = std::move(callback);
}

void InterMsgExample::parse()
{
    if (m_callback)
    {
        m_callback(getClient(), *this);
    }
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

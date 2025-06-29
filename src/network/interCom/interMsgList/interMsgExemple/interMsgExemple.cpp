#include "interMsgExemple.hpp"

#include "tools/logStream/logStream.h"

InterMsgExemple::Callback InterMsgExemple::m_callback = nullptr;

InterMsgExemple::InterMsgExemple(Client client, int exampleData)
    : InterMsg(client, InterMsgData(InterMsgHeader(InterMsgId::Exemple, sizeof(m_data)), (char *)&m_data), m_sendType)
{
    m_data.exampleData = exampleData;
}

InterMsgExemple::InterMsgExemple(Client client, char *rawData, size_t size)
    : InterMsg(client, InterMsgData(rawData, size), m_sendType)
{
}

void InterMsgExemple::registerCallback(Callback callback)
{
    m_callback = std::move(callback);
}

void InterMsgExemple::parse()
{
    if (m_callback)
    {
        m_callback(getClient(), *this);
    }
}

int InterMsgExemple::getExampleData() const
{
    return m_data.exampleData;
}

std::string InterMsgExemple::toString() const
{
    return "InterMsgExemple: exampleData = " + std::to_string(m_data.exampleData)
           + ", client = " + getClient().getIP().getIpString();
}

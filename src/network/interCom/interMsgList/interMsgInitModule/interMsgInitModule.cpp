#include "interMsgInitModule.hpp"

#include "tools/logStream/logStream.h"

InterMsgInitModule::InterMsgInitModule(Client client, KitConfig kitConfig)
    : InterMsg(client, InterMsgData(InterMsgHeader(InterMsgId::InitModule, sizeof(m_data)), (char *)&m_data), m_sendType)
{
    m_data.m_kitConfig = kitConfig;
}

InterMsgInitModule::InterMsgInitModule(Client client, char *rawData, uint32_t size)
    : InterMsg(client, InterMsgData(rawData, size, (char *)&m_data), m_sendType)
{
}

KitConfig InterMsgInitModule::getKitConfig() const
{
    return m_data.m_kitConfig;
}

std::string InterMsgInitModule::toString() const
{
    return "InterMsgInitModule: kitConfig = " + m_data.m_kitConfig.toString() +
           ", client = " + getClient().getIP().getIpString();
}

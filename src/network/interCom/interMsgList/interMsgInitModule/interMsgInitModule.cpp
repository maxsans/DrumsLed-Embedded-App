#include "interMsgInitModule.hpp"

#include "tools/logStream/logStream.h"

InterMsgInitModule::InterMsgInitModule(Client client, uint8_t moduleType)
    : InterMsg(client, InterMsgData(InterMsgHeader(InterMsgId::InitModule, sizeof(m_data)), (char *)&m_data), m_sendType)
{
    m_data.moduleType = moduleType;
}

InterMsgInitModule::InterMsgInitModule(Client client, char *rawData, uint32_t size)
    : InterMsg(client, InterMsgData(rawData, size, (char *)&m_data), m_sendType)
{
}

uint8_t InterMsgInitModule::getModuleType() const
{
    return m_data.moduleType;
}

std::string InterMsgInitModule::toString() const
{
    return "InterMsgInitModule: type = " + std::to_string(m_data.moduleType) +
           ", client = " + getClient().getIP().getIpString();
}

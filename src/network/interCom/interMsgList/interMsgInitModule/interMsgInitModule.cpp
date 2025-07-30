#include "interMsgInitModule.hpp"

#include "tools/logStream/logStream.hpp"

InterMsgInitModule::InterMsgInitModule(Client client, KitConfig kitConfig)
    : InterMsg(
          client,
          InterMsgData(InterMsgHeader(InterMsgId::InitModule, sizeof(Data)),
                       (char *)&m_data),
          m_sendType)
{
    m_data.m_kitConfig = kitConfig;
}

InterMsgInitModule::InterMsgInitModule(Client client,
                                       char *rawData,
                                       uint32_t size)
    : InterMsg(client, InterMsgData(rawData, size, (char *)&m_data), m_sendType)
{
}

const char *InterMsgInitModule::getPrivData() const
{
    return reinterpret_cast<const char *>(&m_data);
}

KitConfig InterMsgInitModule::getKitConfig() const
{
    return m_data.m_kitConfig;
}

std::string InterMsgInitModule::toString() const
{
    return "InterMsgInitModule: kitConfig = " + m_data.m_kitConfig.toString()
           + ", client = " + getClient().getIP().getIpString();
}

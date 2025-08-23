#include "interMsgInitModule.hpp"

#include "tools/logStream/logStream.hpp"
#include <cstring>

InterMsgInitModule::InterMsgInitModule(Client client, KitConfig kitConfig)
    : InterMsg(client, InterMsgId::InitModule, m_sendType)
{
    m_kitConfig = kitConfig;
}

InterMsgInitModule::InterMsgInitModule(MessageReceivedCallback callback)
    : InterMsg(callback, InterMsgId::InitModule)
{
}

uint32_t InterMsgInitModule::serializePriv(char *data) const
{
    // Serialize the kit configuration using network byte order
    m_kitConfig.serialize(data);
    return m_kitConfig.getSerializedSize();
}

uint32_t InterMsgInitModule::deserializePriv(const char *data)
{
    // Deserialize the kit configuration using network byte order
    m_kitConfig = KitConfig(data);
    return m_kitConfig.getSerializedSize();
}

KitConfig InterMsgInitModule::getKitConfig() const
{
    return m_kitConfig;
}

std::string InterMsgInitModule::toString() const
{
    return "InterMsgInitModule: kitConfig = " + m_kitConfig.toString()
           + ", client = " + getClient().getIP().getIpString();
}

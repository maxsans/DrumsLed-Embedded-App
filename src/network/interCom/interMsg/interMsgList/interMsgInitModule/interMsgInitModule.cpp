#include "interMsgInitModule.hpp"

#include "tools/logStream/logStream.hpp"

InterMsgInitModule::InterMsgInitModule(Client client, KitConfig kitConfig)
    : InterMsg(client, InterMsgId::InitModule, m_sendType)
{
    m_data.m_kitConfig = kitConfig;
}

InterMsgInitModule::InterMsgInitModule(MessageReceivedCallback callback)
    : InterMsg(callback, InterMsgId::InitModule)
{
}

uint32_t InterMsgInitModule::serializePriv(char *data) const
{
    // TODO: something like that :
    // Serialize the kit configuration
    /// uint32_t kitConfigSize = m_data.m_kitConfig.serialize(data + sizeof(uint32_t));
    /// std::memcpy(data, &kitConfigSize, sizeof(kitConfigSize));
    /// return sizeof(kitConfigSize) + kitConfigSize;
    return 0;
}

uint32_t InterMsgInitModule::deserializePriv(const char *data)
{
    // TODO: something like that :
    // Deserialize the kit configuration
    /// uint32_t kitConfigSize;
    /// std::memcpy(&kitConfigSize, data, sizeof(kitConfigSize));
    /// m_data.m_kitConfig.deserialize(data + sizeof(kitConfigSize));
    /// return sizeof(kitConfigSize) + kitConfigSize;
    return 0;
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

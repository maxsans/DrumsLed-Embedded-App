#include "module.hpp"
#include "tools/timeTools/timeMs.hpp"
#include "network/interCom/interComParser/interComParser.hpp"
#include "network/interCom/interMsgList/interMsgAlive/interMsgAlive.hpp"

const timeMs Module::m_moduleTimeout = timeMs(5000);

Module::Module(KitConfig kitConfig, Client client) :
    m_kitConfig(kitConfig),
    m_client(client),
    m_lastSyncTime(0)
{
    // Add the attributes based on the kit configuration
    if (kitConfig.hasAttributeType(KitAttributeType::Type::LevelAdc))
    {
        m_micro = new Micro();
    }
    else
    {
        m_micro = nullptr;
    }
    if (kitConfig.hasAttributeType(KitAttributeType::Type::Rgb))
    {
        m_rgbLed = new RgbLed();
    }
    else
    {
        m_rgbLed = nullptr;
    }

    // Register the Alive message callback
    InterComParser::registerCallback(
        InterMsgId::Alive,
        [](const Client&, InterMsg &msg, void *object)
        {
            static_cast<Module*>(object)->onAliveMsg(msg);
        },
        this
    );
}

void Module::onAliveMsg(InterMsg &msg)
{
    // The module is alive, update the last sync time
    sync();
}

Micro *Module::getMicro()
{
    return m_micro;
}

RgbLed *Module::getRgbLed()
{
    return m_rgbLed;
}

bool Module::isConnected()
{
    return m_lastSyncTime != 0
    && m_lastSyncTime + timeMs::nowMs() < m_moduleTimeout;
}

KitConfig Module::getConfig()
{
    return m_kitConfig;
}

void Module::sync()
{
    m_lastSyncTime = timeMs::nowMs();
}

Client Module::getClient()
{
    return m_client;
}

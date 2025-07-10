#include "module.hpp"
#include "tools/timeTools/timeMs.hpp"
#include "network/interCom/interComParser/interComParser.hpp"

const timeMs Module::m_moduleTimeout = timeMs(5000);

Module::Module(KitConfig kitConfig, Client client) :
    m_kitConfig(kitConfig),
    m_client(client),
    m_lastSyncTime(0)
{
    // Register the callback for ADC messages
    InterComParser::registerCallback(m_client, InterMsgId::Adc,
        [this](const Client &client, InterMsg &msg, void *object) {
            this->onAdcMsg(msg);
        });
}

void Module::onAdcMsg(InterMsg &msg)
{
    // TODO: Real implementation
}

Micro *Module::getMicro()
{
    // TODO: Real implementation
    return nullptr;
}

RgbLed *Module::getRgbLed()
{
    // TODO: Real implementation
    return nullptr;
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

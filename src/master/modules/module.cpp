#include "module.hpp"
#include "network/interCom/interComParser/interComParser.hpp"
#include "network/interCom/interMsgList/interMsgRgb/interMsgRgb.hpp"
#include "tools/timeTools/timeMs.hpp"

const TimeMs Module::m_moduleTimeout = TimeMs(5000);
const TimeMs Module::m_rgbSendInterval = TimeMs(20);

Module::Module(KitConfig kitConfig, Client client)
    : m_kitConfig(kitConfig), m_client(client),
      m_aliveRgbPeriodicCall(m_rgbSendInterval, &Module::sendRgb, this),
      m_micro(nullptr), m_rgbLed(nullptr)
{
    // Add the attributes based on the kit configuration
    if (kitConfig.hasAttributeType(KitAttributeType::Type::LevelAdc))
    {
        m_micro = new Micro();
    }
    if (kitConfig.hasAttributeType(KitAttributeType::Type::Rgb))
    {
        m_rgbLed = new RgbLed();
    }

    // At start, the module is connected
    sync();
}

void Module::sendRgb(void *object)
{
    static_cast<Module *>(object)->sendRgb();
}

void Module::sendRgb()
{
    if (m_rgbLed)
    {
        // Send the RGB message to the kits
        InterMsgRgb msgRgb(m_client,
                           m_rgbLed->getColor().getRed(),
                           m_rgbLed->getColor().getGreen(),
                           m_rgbLed->getColor().getBlue());
        msgRgb.send();
    }
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
    return (m_lastSyncTime + m_moduleTimeout) > TimeMs::nowMs();
}

KitConfig Module::getConfig()
{
    return m_kitConfig;
}

void Module::sync()
{
    m_lastSyncTime = TimeMs::nowMs();
}

Client Module::getClient()
{
    return m_client;
}

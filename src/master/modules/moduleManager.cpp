#include "moduleManager.hpp"
#include "network/interCom/interMsgList/interMsgAdc/interMsgAdc.hpp"
#include "network/interCom/interMsgList/interMsgInitModule/interMsgInitModule.hpp"
#include "network/interCom/interMsgList/interMsgPingSlaves/interMsgPingSlaves.hpp"
#include "tools/logStream/logStream.hpp"

#include <stdint.h>
#include <sys/time.h>

const TimeMs ModuleManager::m_ringInterval = TimeMs(500);

ModuleManager::ModuleManager(bool active)
    : m_ringPeriodicCalls(m_ringInterval, [this]() { this->ringModules(); }),
      m_active(active), m_impactsManager()
{
    enable(active);

    // Initialize the modules vector
    m_modules.clear();

    // Register the callback to push back the module when a new module is detected
    InterComParser::registerCallback(
        InterMsgId::InitModule,
        [this](const Client &client, InterMsg &msg, void *object) {
            this->onNewModule(client, msg);
        });

    // Register the callback to handle ADC messages
    InterComParser::registerCallback(
        InterMsgId::Adc,
        [this](const Client &client, InterMsg &msg, void *object) {
            this->onAdcMsg(msg);
            return;
        });
}

void ModuleManager::enable(bool e)
{
    m_active = e;
}

void ModuleManager::onNewModule(const Client &client, InterMsg &msg)
{
    // Add the new module to the module manager
    InterMsgInitModule *initMsg = static_cast<InterMsgInitModule *>(&msg);
    if (initMsg != nullptr)
    {
        addModule(initMsg->getKitConfig(), client);
    }
    else
    {
        LogStream::cout << "Received InitModule message with invalid type: "
                        << msg.toString() << LogStream::endl;
    }
}

void ModuleManager::onAdcMsg(InterMsg &msg)
{
    // Get the client from the message
    Client client = msg.getClient();
    // Find the module associated with the client
    Module *l_module = getModule(client);
    // Check if the module exists and have a micro
    if (l_module != nullptr && l_module->getMicro() != nullptr)
    {
        // Get the ADC value from the message
        InterMsgAdc *adcMsg = static_cast<InterMsgAdc *>(&msg);
        adc_measure_t adcValue = adcMsg->getAdcValue();
        // Set the raw ADC value to the micro
        l_module->getMicro()->setMicroValue(adcValue);
        // Calculate the corrected ADC value
        m_impactsManager.setMicroValue(l_module->getMicro(), adcValue);
    }
}

bool ModuleManager::isActive()
{
    return m_active;
}

Module *ModuleManager::addModule(KitConfig kitConfig, Client client)
{
    // Check if the module already exists
    Module *existingModule = getModule(client);
    if (existingModule == nullptr)
    {
        if (m_active)
        {
            // Create a new module and add it to the list
            m_modules.push_back(new Module(kitConfig, client));
            // Log the addition of the module
            LogStream::cout << "Module added: " << client.getIP().getIpString()
                            << LogStream::endl;
        }
    }
    else
    {
        // Sync the existing module
        existingModule->sync();
    }
    return nullptr;
}

Module *ModuleManager::getModule(int32_t index)
{
    if (index < m_modules.size())
    {
        return m_modules[index];
    }
    return nullptr;
}

Module *ModuleManager::getModule(Client client)
{
    for (int32_t i = 0; i < m_modules.size(); i++)
    {
        if (m_modules[i]->getClient() == client)
        {
            return m_modules[i];
        }
    }
    return nullptr;
}

Module *ModuleManager::getModule(Ipv4 ip)
{
    for (int32_t i = 0; i < m_modules.size(); i++)
    {
        if (m_modules[i]->getClient().getIP() == ip)
        {
            return m_modules[i];
        }
    }
    return nullptr;
}

Module *ModuleManager::getModule(MacAddr mac)
{
    for (int32_t i = 0; i < m_modules.size(); i++)
    {
        if (m_modules[i]->getClient().getMAC() == mac)
        {
            return m_modules[i];
        }
    }
    return nullptr;
}

uint32_t ModuleManager::getModuleCount()
{
    return m_modules.size();
}

void ModuleManager::ringModules()
{
    // Check if the modules are still connected
    for (int32_t i = m_modules.size() - 1; i >= 0; i--)
    {
        Module *l_module = m_modules[i];
        if (!l_module->isConnected())
        {
            LogStream::cout << "Module "
                            << l_module->getClient().getIP().getIpString()
                            << " disconnected." << LogStream::endl;
            // Remove the module from the list
            delete l_module;
            m_modules.erase(m_modules.begin() + i);
        }
    }
    // Send a broadcast UDP packet to ring new modules
    // The modules detected will respond
    InterMsgPingSlaves().send();
}

void ModuleManager::setMicro(Client client, uint8_t microValue)
{
    // Find the module of the client
    Module *l_module = getModule(client);

    // Get the micro of the module
    Micro *l_micro = l_module->getMicro();
    if (l_micro == nullptr)
    {
        LogStream::cout << "No micro found for this module" << LogStream::endl;
        return;
    }

    // Set the micro value (not corrected)
    l_micro->setMicroValue(microValue);

    // Set the corrected value of the micro
    m_impactsManager.setMicroValue(l_micro, microValue);
}

ImpactsManager *ModuleManager::getImpactsManager()
{
    return &m_impactsManager;
}

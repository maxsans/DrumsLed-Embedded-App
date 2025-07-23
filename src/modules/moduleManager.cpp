#include "moduleManager.hpp"
#include "network/interCom/interMsgList/interMsgPingSlaves/interMsgPingSlaves.hpp"
#include "tools/logStream/logStream.hpp"

#include <stdint.h>
#include <sys/time.h>

const timeMs ModuleManager::m_ringInterval = timeMs(500);

ModuleManager::ModuleManager(bool active) :
    m_ringPeriodicCalls(m_ringInterval, ringCallback, this)
{
    enable(active);

    // Initialize the modules vector
    m_modules.clear();
}

void ModuleManager::enable(bool e)
{
    m_active = e;
    if (m_active)
    {
        // Start the periodic calls to ring the modules
        m_ringPeriodicCalls.enable(true);
    }
    else
    {
        // Stop the periodic calls to ring the modules
        m_ringPeriodicCalls.enable(false);
    }
}

bool ModuleManager::isActive()
{
    return m_active;
}

Module *ModuleManager::addModule(KitConfig kitConfig, Client client)
{
    if (m_active)
    {
        m_modules.push_back(new Module(kitConfig, client));
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

void ModuleManager::ringCallback(void *object)
{
    ((ModuleManager *)object)->ringModules();
}

void ModuleManager::ringModules()
{
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
    if(l_micro == nullptr)
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

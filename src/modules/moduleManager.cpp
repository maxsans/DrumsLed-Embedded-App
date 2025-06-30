#include "moduleManager.h"
#include "modules/drumModule.h"
#include "modules/cymbalModule.h"
#include "network/interCom/interMsgList/interMsgPingSlaves/interMsgPingSlaves.hpp"
#include "tools/logStream/logStream.h"

#include <stdint.h>
#include <sys/time.h>

#define RING_INTERVAL 500 // ms

std::vector<Module *> ModuleManager::m_modules;
bool ModuleManager::m_enableNewModules = true;
periodicCallsMs *ModuleManager::m_ringPeriodicCalls;
ImpactsManager ModuleManager::m_impactsManager;

void ModuleManager::init()
{
    // Initialize the periodic calls
    m_ringPeriodicCalls = new periodicCallsMs(RING_INTERVAL, ringCallback, NULL);
}

void ModuleManager::process()
{
    // Process the modules
    for (int32_t i = 0; i < m_modules.size(); i++)
    {
        m_modules[i]->process();
    }
}

void ModuleManager::enableNewModules(bool enable)
{
    m_enableNewModules = enable;
}

bool ModuleManager::NewModulesEnabled()
{
    return m_enableNewModules;
}

Module *ModuleManager::addModule(moduleType_t type, Client client)
{
    if (m_enableNewModules)
    {
        switch (type)
        {
        case TYPE_DRUM_MODULE:
            m_modules.push_back(new DrumModule(client));
            return m_modules[m_modules.size() - 1];
            break;

        case TYPE_CYMBAL_MODULE:
            m_modules.push_back(new CymbalModule(client));
            return m_modules[m_modules.size() - 1];
            break;

        default:
            LogStream::cout << "Module type not supported" << LogStream::endl;
            break;
        }
    }
    return NULL;
}

Module *ModuleManager::getModule(int32_t index)
{
    if (index < m_modules.size())
    {
        return m_modules[index];
    }
    return NULL;
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
    return NULL;
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
    return NULL;
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
    return NULL;
}

Module *ModuleManager::getModule(Micro *micro)
{
    for (int32_t i = 0; i < m_modules.size(); i++)
    {
        if (m_modules[i]->getMicro() == micro)
        {
            return m_modules[i];
        }
    }
    return NULL;
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

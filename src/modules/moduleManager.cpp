#include "moduleManager.h"
#include "network/udpParser/udpPackets/udpPacketPingSlaves.h"

#include <stdint.h>
#include <sys/time.h>

#define RING_INTERVAL 500 // ms


ModuleManager::ModuleManager()
{
    m_enableNewModules = true;
    // Send a broadcast UDP packet to ring new modules every RING_INTERVAl ms
    m_ringPeriodicCalls.setPeriod(RING_INTERVAL);
    m_ringPeriodicCalls.setCallback(ringCallback, this);
}

ModuleManager::~ModuleManager()
{

}

void ModuleManager::enableNewModules(bool enable)
{
    m_enableNewModules = enable;
}

bool ModuleManager::NewModulesEnabled()
{
    return m_enableNewModules;
}

bool ModuleManager::addModule(Module *m)
{
    if (m_enableNewModules)
    {
        m_modules.push_back(m);
        return true;
    }
    return false;
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
    UdpPacketPingSlaves().send();
}

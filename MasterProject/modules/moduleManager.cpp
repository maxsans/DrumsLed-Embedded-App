#include "moduleManager.h"
#include "udp.h"
#include "udpPacket.h"

#include <stdint.h>
#include <stdio.h>
#include <sys/time.h>

#define RING_INTERVAL 500 // ms


moduleManager::moduleManager()
{
    m_enableNewModules = true;
    // Send a broadcast UDP packet to ring new modules every RING_INTERVAl ms
    m_ringPeriodicCalls.setPeriod(RING_INTERVAL);
    m_ringPeriodicCalls.setCallback(ringCallback, this);
}

moduleManager::~moduleManager()
{

}

void moduleManager::enableNewModules(bool enable)
{
    m_enableNewModules = enable;
}

bool moduleManager::NewModulesEnabled()
{
    return m_enableNewModules;
}

bool moduleManager::addModule(module *m)
{
    if (m_enableNewModules)
    {
        m_modules.push_back(m);
        return true;
    }
    return false;
}

module *moduleManager::getModule(int32_t index)
{
    if (index < m_modules.size())
    {
        return m_modules[index];
    }
    return NULL;
}

module *moduleManager::getModule(Client client)
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

module *moduleManager::getModule(IPv4 ip)
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

module *moduleManager::getModule(MacAddr mac)
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

uint32_t moduleManager::getModuleCount()
{
    return m_modules.size();
}

void moduleManager::ringCallback(void *object)
{
    ((moduleManager *)object)->ringModules();
}


void moduleManager::ringModules()
{
    // Send a broadcast UDP packet to ring new modules
    // The modules detected will respond
    char l_packet[] =
    {
        PACKET_TYPE_INIT // Ring packet
    };
    g_udp.sendbroadcast(l_packet, sizeof(l_packet));
}

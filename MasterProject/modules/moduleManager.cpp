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

void moduleManager::process()
{
    // Send a broadcast UDP packet to ring new modules every RING_INTERVAl ms
    static uint64_t l_lastRingTime = GetTickCount64();
    if (GetTickCount64() - l_lastRingTime > RING_INTERVAL)
    {
        ringModules();
        l_lastRingTime = GetTickCount64();
    }
}

bool moduleManager::addModule(char* ip)
{
    if (!m_enableNewModules)
    {
        return false;
    }
    // Check if the module already exists
    for (int32_t i = 0; i < m_modules.size(); i++)
    {
        if (strcmp(m_modules[i]->getIp(), ip) == 0)
        {
            return false; // Already exists, dont add again
        }
    }
    m_modules.push_back(new module(ip));
    return true;
}

module *moduleManager::getModule(int32_t index)
{
    return m_modules[index];
}

module *moduleManager::getModule(char* ip)
{
    for (int32_t i = 0; i < m_modules.size(); i++)
    {
        if (strcmp(m_modules[i]->getIp(), ip) == 0)
        {
            return m_modules[i];
        }
    }

    return NULL;
}

int moduleManager::getModuleCount()
{
    return m_modules.size();
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

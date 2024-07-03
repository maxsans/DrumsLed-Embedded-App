#include "moduleManager.h"
#include "udp.h"

#include <stdint.h>
#include <stdio.h>

moduleManager::moduleManager()
{

}

moduleManager::~moduleManager()
{

}

bool moduleManager::addModule(char* ip)
{
    // Check if the module already exists
    for (int32_t i = 0; i < m_modules.size(); i++)
    {
        if (strcmp(m_modules[i].getip(), ip) == 0)
        {
            return false; // Already exists, dont add again
        }
    }
    module l_module;
    l_module.setip(ip);
    m_modules.push_back(l_module);
    return true;
}

module *moduleManager::getModule(int32_t index)
{
    return &m_modules[index];
}

module *moduleManager::getModule(char* ip)
{
    for (int32_t i = 0; i < m_modules.size(); i++)
    {
        if (strcmp(m_modules[i].getip(), ip) == 0)
        {
            return &m_modules[i];
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
        1 // Ring packet
    };
    g_udp.sendbroadcast(l_packet);
}

moduleManager g_moduleManager;

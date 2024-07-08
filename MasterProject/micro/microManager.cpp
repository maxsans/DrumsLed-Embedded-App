#include "microManager.h"

#include <assert.h>

microManager::microManager()
{
    // Constructor
}

microManager::~microManager()
{
    // Destructor
    for (uint8_t i = 0; i < m_micros.size(); i++)
    {
        delete m_micros[i];
    }
}

void microManager::addMicro(module *m)
{
    // Check if the micro already exists
    for (uint8_t i = 0; i < m_micros.size(); i++)
    {
        if (m_micros[i]->getModule() == m)
        {
            return; // Already exists, dont add again
        }
    }
    // Add a new micro to the manager
    m_micros.push_back(new micro(m));
}

micro *microManager::getMicro(int32_t index)
{
    // Get a micro by index
    assert(index >= 0 && index < m_micros.size());
    return m_micros[index];
}

micro *microManager::getMicro(module *m)
{
    // Get a micro by module
    for (uint8_t i = 0; i < m_micros.size(); i++)
    {
        if (m_micros[i]->getModule() == m)
        {
            return m_micros[i];
        }
    }
    assert(false); // Micro not found
    return NULL;
}

uint32_t microManager::getMicroCount()
{
    // Get the number of micros
    return m_micros.size();
}

microManager g_microManager;

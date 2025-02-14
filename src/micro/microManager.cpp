#include "microManager.h"

#include <assert.h>

MicroManager::MicroManager()
{
    // Constructor
}

MicroManager::~MicroManager()
{
    // Destructor
    for (uint8_t i = 0; i < m_micros.size(); i++)
    {
        delete m_micros[i];
    }
}

void MicroManager::process()
{
    // Process all micros
    for (uint8_t i = 0; i < m_micros.size(); i++)
    {
        m_micros[i]->process();
    }
}

void MicroManager::addMicro(Module *m)
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
    m_micros.push_back(new Micro(m));
    // Add a new micro to the impacts manager
    m_impactsManager.addMicro();
}

void MicroManager::setMicro(Module *m, uint8_t microValue)
{
    // Set the value of a micro
    // Find the micro from his module
    Micro *m_micro = getMicro(m);
    m_micro->setMicroValue(microValue);
    // Find the index of the micro
    uint32_t l_microIndex = 0;
    for (uint32_t i = 0; i < m_micros.size(); i++)
    {
        if (m_micros[i] == m_micro)
        {
            l_microIndex = i;
            break;
        }
    }
    // Set the corrected value of the micro
    // First, remove the impact of all other micros one this one
    int32_t l_microValueCorrected = microValue;
    for (uint32_t l_ImpactorMicroIndex = 0; l_ImpactorMicroIndex < m_micros.size(); l_ImpactorMicroIndex++)
    {
        if (m_micros[l_ImpactorMicroIndex]->getModule() != m)
        {
            float l_impact = m_impactsManager.getArtImpact(l_ImpactorMicroIndex, l_microIndex);
            uint8_t l_impactorMicroValue = m_micros[l_ImpactorMicroIndex]->getMicroValue();
            l_microValueCorrected -= l_impactorMicroValue * l_impact ;
        }
    }
    if (l_microValueCorrected < 0)
    {
        l_microValueCorrected = 0;
    }
    // Then, apply the correction
    l_microValueCorrected = l_microValueCorrected * m_micro->getCorrection();
    // Finally, set the corrected value
    m_micro->setMicroValueCorrected(l_microValueCorrected);
}

Micro *MicroManager::getMicro(int32_t index)
{
    // Get a micro by index
    assert(index >= 0 && index < m_micros.size());
    return m_micros[index];
}

Micro *MicroManager::getMicro(Module *m)
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

uint32_t MicroManager::getMicroCount()
{
    // Get the number of micros
    return m_micros.size();
}

impactsManager *MicroManager::getImpactsManager()
{
    // Get the impacts manager
    return &m_impactsManager;
}

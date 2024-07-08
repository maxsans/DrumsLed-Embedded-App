#include "impactsManager.h"

#include <stdio.h>

impactsManager::impactsManager()
{

}

void impactsManager::addMicro()
{
    // Add a new micro on the two dimensions of the vector
    // x x x          x x x x
    // x x x    ->    x x x x
    // x x x          x x x x
    //                x x x x
    m_impacts.push_back(std::vector<impact *>(m_impacts.size(), new impact()));
    for (uint32_t i = 0; i < m_impacts.size(); i++)
    {
        m_impacts[i].push_back(new impact());
    }
}

coeff impactsManager::getRealImpact(uint32_t impactorMicro, uint32_t impactedMicro)
{
    return m_impacts[impactorMicro][impactedMicro]->m_realImpact;
}

coeff impactsManager::getArtImpact(uint32_t impactorMicro, uint32_t impactedMicro)
{
    return m_impacts[impactorMicro][impactedMicro]->m_ArtImpact;
}

impact *impactsManager::getImpact(uint32_t impactorMicro, uint32_t impactedMicro)
{
    return m_impacts[impactorMicro][impactedMicro];
}

void impactsManager::setRealImpact(uint32_t impactorMicro, uint32_t impactedMicro, coeff impact)
{
    m_impacts[impactorMicro][impactedMicro]->m_realImpact = impact;
}

void impactsManager::setImpact(uint32_t impactorMicro, uint32_t impactedMicro, impact impact)
{
    *m_impacts[impactorMicro][impactedMicro] = impact;
}

void impactsManager::calculateArtImpacts()
{
    // TODO
}


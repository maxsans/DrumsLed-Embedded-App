#include "impactsManager.hpp"
#include "tools/maths/matrix/matrix.hpp"

#include <assert.h>

ImpactsManager::ImpactsManager()
{
}

Micro *ImpactsManager::getMicro(uint32_t index)
{
    assert(index < m_micros_table.size());
    return m_micros_table[index];
}

uint32_t ImpactsManager::getMicroIndex(Micro *micro)
{
    for (uint32_t i = 0; i < m_micros_table.size(); i++)
    {
        if (m_micros_table[i] == micro)
        {
            return i;
        }
    }
    return -1;
}

void ImpactsManager::addMicro(Micro *micro)
{
    // Add a new micro in the micros table
    m_micros_table.push_back(micro);

    // Add a new micro on the two dimensions of the vector
    // x x x          x x x x
    // x x x    ->    x x x x
    // x x x          x x x x
    //                x x x x
    // First add a new impact on each olders vectors
    for (uint32_t i = 0; i < m_impacts.size(); i++)
    {
        m_impacts[i]->push_back(new Impact());
    }
    uint32_t l_newSize = m_impacts.size() + 1;
    // Then add a new vector of impacts on the first dimension
    m_impacts.push_back(new std::vector<Impact *>(l_newSize));
    assert(m_impacts.size() == l_newSize);
    for (uint32_t i = 0; i < l_newSize; i++)
    {
        // Fill the new vector with new impacts
        m_impacts[l_newSize - 1]->at(i) = new Impact();
        // Check that the sizes are correct
        uint32_t l_currentSize = m_impacts[i]->size();
        assert(l_currentSize == l_newSize);
    }
}

float ImpactsManager::getRealImpact(Micro *impactorMicro, Micro *impactedMicro)
{
    uint32_t l_impactorIndex = getMicroIndex(impactorMicro);
    uint32_t l_impactedIndex = getMicroIndex(impactedMicro);
    assert(l_impactorIndex < m_impacts.size()
           && l_impactedIndex < m_impacts.size() && l_impactorIndex >= 0
           && l_impactedIndex >= 0);
    return m_impacts[l_impactorIndex]->at(l_impactedIndex)->m_realImpact;
}

float ImpactsManager::getArtImpact(Micro *impactorMicro, Micro *impactedMicro)
{
    uint32_t l_impactorIndex = getMicroIndex(impactorMicro);
    uint32_t l_impactedIndex = getMicroIndex(impactedMicro);
    assert(l_impactorIndex < m_impacts.size()
           && l_impactedIndex < m_impacts.size() && l_impactorIndex >= 0
           && l_impactedIndex >= 0);
    return m_impacts[l_impactorIndex]->at(l_impactedIndex)->m_ArtImpact;
}

void ImpactsManager::setRealImpact(Micro *impactorMicro,
                                   Micro *impactedMicro,
                                   float impact)
{
    uint32_t l_impactorIndex = getMicroIndex(impactorMicro);
    uint32_t l_impactedIndex = getMicroIndex(impactedMicro);
    assert(l_impactorIndex < m_impacts.size()
           && l_impactedIndex < m_impacts.size() && l_impactorIndex >= 0
           && l_impactedIndex >= 0);
    m_impacts[l_impactorIndex]->at(l_impactedIndex)->m_realImpact = impact;
}

void ImpactsManager::setImpact(Micro *impactorMicro,
                               Micro *impactedMicro,
                               Impact impact)
{
    uint32_t l_impactorIndex = getMicroIndex(impactorMicro);
    uint32_t l_impactedIndex = getMicroIndex(impactedMicro);
    assert(l_impactorIndex < m_impacts.size()
           && l_impactedIndex < m_impacts.size() && l_impactorIndex >= 0
           && l_impactedIndex >= 0);
    m_impacts[l_impactorIndex]->at(l_impactedIndex)->m_realImpact
        = impact.m_realImpact;
    m_impacts[l_impactorIndex]->at(l_impactedIndex)->m_ArtImpact
        = impact.m_ArtImpact;
}

uint32_t ImpactsManager::impactRank(uint32_t impactorMicro,
                                    uint32_t impactedMicro)
{
    assert(impactorMicro != impactedMicro && impactorMicro < m_impacts.size()
           && impactedMicro < m_impacts.size() && impactorMicro >= 0
           && impactedMicro >= 0);
    // Genarate each link between two micros (except the link of a micro on itself)
    // The rank of the link is the number of links before the link
    uint32_t l_nbMicros = m_impacts.size();
    uint32_t l_rank = 0;
    for (uint32_t l_impactorMicro = 0; l_impactorMicro < l_nbMicros;
         l_impactorMicro++)
    {
        for (uint32_t l_impactedMicro = 0; l_impactedMicro < l_nbMicros;
             l_impactedMicro++)
        {
            if (l_impactorMicro != l_impactedMicro)
            {
                if (l_impactorMicro == impactorMicro
                    && l_impactedMicro == impactedMicro)
                {
                    return l_rank;
                }
                l_rank++;
            }
        }
    }
    return l_rank;
}

void ImpactsManager::calculateArtImpacts()
{
    /*
        Each micro has an impact on the other micros

        Impacts are a ratio of the real impact of a micro on an other micro

        So we need to subtract a part on each micro
        This part depend on the real impacts of the micros
        but it's not directly the real impact

        Indeed, the real impact is the impact of a micro on an other micro
        So if we subtract the real impact of each micro on each other micro
        we will subtract too much :
        We play with micro A.
        A will raise up B and C
        So if we subtract the real impact of A on B and C to B, be will be too low

        So we need to determinate artificial impacts as it works
        To determinate the artificial impacts, we need to solve a system of equations

        We will use the folllowing rating :
        AB is the real impact of A on B
        ab is the artificial impact of A on B

        Exemple of system of equations with 3 micros :
            | AB = ab + AC*cb
            | AC = ac + AB*bc
            | BA = ba + BC*ca
            | BC = bc + BA*ac
            | CA = ca + CB*ba
            | CB = cb + CA*ab

        Exemple of system of equations with 4 micros :
            | AB = ab + AC*cb + AD*db
            | AC = ac + AB*bc + AD*dc
            | AD = ad + AB*bd + AC*cd
            | BA = ba + BC*cb + BD*db
            | BC = bc + BA*ab + BD*dc
            | BD = bd + BA*ad + BC*cd
            | CA = ca + CB*bc + CD*dc
            | CB = cb + CA*ac + CD*dc
            | CD = cd + CA*ad + CB*bd
            | DA = da + DB*bd + DC*cd
            | DB = db + DA*ad + DC*cd
            | DC = dc + DA*ad + DB*bd

        So to solve these systems of equations, we will use matrices
        We will convert the system of equations into a matrix

        Exemple of matrix with 3 micros :
            | AB |     | 1    0    0    0    0    AC |     | ab |
            | AC |     | 0    1    0    AB   0    0  |     | ac |
            | BA |     | 0    0    1    0    BC   0  |     | ba |
            | BC |  =  | 0    BA   0    1    0    0  |  *  | bc |
            | CA |     | 0    0    CB   0    1    0  |     | ca |
            | CB |     | CA   0    0    0    0    1  |     | cb |

        And we will invert the matrix to solve the system of equations
        Solutions will be the artificial impacts

    */

    // Get the number of micros
    uint32_t l_nbMicros = m_impacts.size();
    uint32_t l_matrixSize = l_nbMicros * (l_nbMicros - 1);

    // Create the matrix
    matrix l_matrix(l_matrixSize);
    // Fill the matrix
    // Generate each link between two micros
    // This table will be used to find a real impact from its rank
    std::vector<float> l_realImpactsFromRank(l_matrixSize, float());
    // First fill the diagonal with 1
    for (uint32_t i = 0; i < l_matrixSize; i++)
    {
        l_matrix.set(i, i, 1);
    }
    // Fill the matrix
    for (uint32_t l_impactorMicro = 0; l_impactorMicro < l_nbMicros;
         l_impactorMicro++)
    {
        for (uint32_t l_impactedMicro = 0; l_impactedMicro < l_nbMicros;
             l_impactedMicro++)
        {
            // Skip the impact of a micro on itself
            if (l_impactorMicro != l_impactedMicro)
            {
                // Get the rank of the link
                uint32_t l_rank = impactRank(l_impactorMicro, l_impactedMicro);

                l_realImpactsFromRank[l_rank] = m_impacts[l_impactorMicro]
                                                    ->at(l_impactedMicro)
                                                    ->m_realImpact;

                // Generate the middle micros
                for (uint32_t l_otherMicro = 0; l_otherMicro < l_nbMicros;
                     l_otherMicro++)
                {
                    // Skip the impact of a micro on itself
                    if (l_otherMicro != l_impactorMicro
                        && l_otherMicro != l_impactedMicro)
                    {
                        // Get the rank of the other link
                        // Fill the matrix
                        uint32_t l_line
                            = impactRank(l_impactorMicro, l_impactedMicro);
                        uint32_t l_column
                            = impactRank(l_otherMicro, l_impactedMicro);
                        float l_impact = m_impacts[l_impactorMicro]
                                             ->at(l_otherMicro)
                                             ->m_realImpact;
                        l_matrix.set(l_line, l_column, l_impact);
                    }
                }
            }
        }
    }

    // Invert the matrix
    l_matrix.invert();

    // Fill the artificial impacts
    for (uint32_t l_impactorMicro = 0; l_impactorMicro < l_nbMicros;
         l_impactorMicro++)
    {
        for (uint32_t l_impactedMicro = 0; l_impactedMicro < l_nbMicros;
             l_impactedMicro++)
        {
            // Skip the impact of a micro on itself
            if (l_impactorMicro == l_impactedMicro)
            {
                m_impacts[l_impactorMicro]->at(l_impactedMicro)->m_ArtImpact
                    = 1;
            }
            else
            {
                uint32_t l_line = impactRank(l_impactorMicro, l_impactedMicro);
                m_impacts[l_impactorMicro]->at(l_impactedMicro)->m_ArtImpact
                    = 0;
                for (uint32_t l_workColumn = 0; l_workColumn < l_matrixSize;
                     l_workColumn++)
                {
                    m_impacts[l_impactorMicro]->at(l_impactedMicro)->m_ArtImpact
                        += l_matrix.get(l_line, l_workColumn)
                           * l_realImpactsFromRank[l_workColumn];
                }
            }
        }
    }
}

void ImpactsManager::setMicroValue(Micro *micro, uint8_t microValue)
{
    // Get the index of the micro
    uint32_t l_impactedMicroIndex = getMicroIndex(micro);
    // Remove the impact of all other micros one this one
    int32_t l_microValueCorrected = microValue;
    for (uint32_t l_ImpactorMicroIndex = 0;
         l_ImpactorMicroIndex < m_micros_table.size();
         l_ImpactorMicroIndex++)
    {
        if (l_ImpactorMicroIndex != l_impactedMicroIndex)
        {
            float l_impact = m_impacts[l_ImpactorMicroIndex]
                                 ->at(l_impactedMicroIndex)
                                 ->m_ArtImpact;
            uint8_t l_impactorMicroValue
                = m_micros_table[l_ImpactorMicroIndex]->getMicroValue();
            l_microValueCorrected -= l_impactorMicroValue * l_impact;
        }
    }
    if (l_microValueCorrected < 0)
    {
        l_microValueCorrected = 0;
    }
    micro->setMicroValueCorrected(l_microValueCorrected);
}

uint32_t ImpactsManager::getMicroCount()
{
    return m_micros_table.size();
}

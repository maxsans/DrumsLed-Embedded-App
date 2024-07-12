#include "impactsManager.h"
#include "matrix.h"

#include <stdio.h>
#include <assert.h>

impactsManager::impactsManager()
{

}

uint32_t impactsManager::getMicroCount()
{
    return m_impacts.size();
}

void impactsManager::addMicro()
{
    // Add a new micro on the two dimensions of the vector
    // x x x          x x x x
    // x x x    ->    x x x x
    // x x x          x x x x
    //                x x x x
    uint32_t l_newSize = m_impacts.size() + 1;
    m_impacts.push_back(new std::vector<impact *>(l_newSize, new impact()));
    assert(m_impacts.size() == l_newSize);
    for (uint32_t i = 0 ; i < l_newSize-1 ; i++)
    {
        m_impacts[i]->push_back(new impact());
        uint32_t l_currentSize = m_impacts[i]->size();
        assert(l_currentSize == l_newSize);
    }
}

coeff impactsManager::getRealImpact(uint32_t impactorMicro, uint32_t impactedMicro)
{
    assert(impactorMicro < m_impacts.size()
        && impactedMicro < m_impacts.size()
        && impactorMicro >= 0
        && impactedMicro >= 0);
    return m_impacts[impactorMicro]->at(impactedMicro)->m_realImpact;
}

coeff impactsManager::getArtImpact(uint32_t impactorMicro, uint32_t impactedMicro)
{
    assert(impactorMicro < m_impacts.size()
        && impactedMicro < m_impacts.size()
        && impactorMicro >= 0
        && impactedMicro >= 0);
    return m_impacts[impactorMicro]->at(impactedMicro)->m_ArtImpact;
}

impact *impactsManager::getImpact(uint32_t impactorMicro, uint32_t impactedMicro)
{
    assert(impactorMicro < m_impacts.size()
        && impactedMicro < m_impacts.size()
        && impactorMicro >= 0
        && impactedMicro >= 0);
    return m_impacts[impactorMicro]->at(impactedMicro);
}

void impactsManager::setRealImpact(uint32_t impactorMicro, uint32_t impactedMicro, coeff impact)
{
    assert(impactorMicro < m_impacts.size()
        && impactedMicro < m_impacts.size()
        && impactorMicro >= 0
        && impactedMicro >= 0);
    m_impacts[impactorMicro]->at(impactedMicro)->m_realImpact = impact;
}

void impactsManager::setImpact(uint32_t impactorMicro, uint32_t impactedMicro, impact impact)
{
    assert(impactorMicro < m_impacts.size()
        && impactedMicro < m_impacts.size()
        && impactorMicro >= 0
        && impactedMicro >= 0);
    *m_impacts[impactorMicro]->at(impactedMicro) = impact;
}

uint32_t impactsManager::impactRank(uint32_t impactorMicro, uint32_t impactedMicro)
{
    assert(impactorMicro != impactedMicro
        && impactorMicro < m_impacts.size()
        && impactedMicro < m_impacts.size()
        && impactorMicro >= 0
        && impactedMicro >= 0);
    // Genarate each link between two micros (except the link of a micro on itself)
    // The rank of the link is the number of links before the link
    uint32_t l_nbMicros = m_impacts.size();
    uint32_t l_rank = 0;
    for (uint32_t l_impactorMicro = 0 ; l_impactorMicro < l_nbMicros ; l_impactorMicro++)
    {
        for (uint32_t l_impactedMicro = 0 ; l_impactedMicro < l_nbMicros ; l_impactedMicro++)
        {
            if (l_impactorMicro != l_impactedMicro)
            {
                if (l_impactorMicro == impactorMicro && l_impactedMicro == impactedMicro)
                {
                    return l_rank;
                }
                l_rank++;
            }
        }
    }
    return l_rank;
}

void impactsManager::calculateArtImpacts()
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
    std::vector<coeff> l_realImpactsFromRank(l_matrixSize, coeff());
    // First fill the diagonal with 1
    for (uint32_t i = 0 ; i < l_matrixSize ; i++)
    {
        l_matrix.set(i, i, coeff(1));
    }
    // Fill the matrix
    for (uint32_t l_impactorMicro = 0 ; l_impactorMicro < l_nbMicros ; l_impactorMicro++)
    {
        for (uint32_t l_impactedMicro = 0 ; l_impactedMicro < l_nbMicros ; l_impactedMicro++)
        {
            // Skip the impact of a micro on itself
            if (l_impactorMicro != l_impactedMicro)
            {
                // Get the rank of the link
                uint32_t l_rank = impactRank(l_impactorMicro, l_impactedMicro);

                l_realImpactsFromRank[l_rank].m_value = m_impacts[l_impactorMicro]->at(l_impactedMicro)->m_realImpact.m_value;

                // Generate the middle micros
                for (uint32_t l_otherMicro = 0 ; l_otherMicro < l_nbMicros ; l_otherMicro++)
                {
                    // Skip the impact of a micro on itself
                    if (l_otherMicro != l_impactorMicro && l_otherMicro != l_impactedMicro)
                    {
                        // Get the rank of the other link
                        // Fill the matrix
                        int l_line = impactRank(l_impactorMicro, l_impactedMicro);
                        int l_column = impactRank(l_otherMicro, l_impactedMicro);
                        l_matrix.set(l_line, l_column, getRealImpact(l_impactorMicro, l_otherMicro));
                    }
                }
            }
        }
    }

    // Invert the matrix
    l_matrix.invert();

    // Fill the artificial impacts
    for (uint32_t l_impactorMicro = 0 ; l_impactorMicro < l_nbMicros ; l_impactorMicro++)
    {
        for (uint32_t l_impactedMicro = 0 ; l_impactedMicro < l_nbMicros ; l_impactedMicro++)
        {
            // Skip the impact of a micro on itself
            if (l_impactorMicro == l_impactedMicro)
            {
                m_impacts[l_impactorMicro]->at(l_impactedMicro)->m_ArtImpact.m_value = QUANTUM_COEFF;
            }
            else
            {
                int l_line = impactRank(l_impactorMicro, l_impactedMicro);
                m_impacts[l_impactorMicro]->at(l_impactedMicro)->m_ArtImpact.m_value = 0;
                for (int l_workColumn=0 ; l_workColumn<l_matrixSize ; l_workColumn++)
                {
                    m_impacts[l_impactorMicro]->at(l_impactedMicro)->m_ArtImpact.m_value
                        += l_matrix.get(l_line, l_workColumn).m_value * l_realImpactsFromRank[l_workColumn].m_value / QUANTUM_COEFF;
                }
            }
        }
    }
}

#include "recordSlot.h"
#include "coeff.h"
#include "impactsManager.h"

#include <assert.h>
#include <stdio.h>

recordSlot::recordSlot(uint32_t nbMicros)
{
    m_records.resize(nbMicros);
    for (uint32_t i = 0; i < nbMicros; i++)
    {
        m_records[i] = new record(nbMicros);
    }
}

recordSlot::~recordSlot()
{
    for (uint32_t i = 0; i < m_records.size(); i++)
    {
        delete m_records[i];
    }
    m_records.clear();
}

void recordSlot::addRecord(record newRecord)
{
    // Control that the record has a good size
    // (all records must have the same size = number of micros)
    if (m_records.size() > 0)
    {
        assert(newRecord.getSize() == m_records[0]->getSize());
    }
    m_records.push_back(new record(newRecord));
}

record recordSlot::getRecord(uint32_t index)
{
    assert(index < m_records.size());
    return *m_records[index];
}

uint32_t recordSlot::getSize()
{
    return m_records.size();
}

void recordSlot::calculateImpacts(impactsManager *impactsManager)
{
    // Calculate the real impacts of all micros on all micros
    // Method :
    // For each micro, calculate the sum of the records
    // Then, calculate the ratio of the sum of the records of the micro
    // with the sum of the records of all the other micros
    // This will give us the real impacts of all micros on all the other micros

    assert(m_records.size() > 0);
    uint32_t l_nbMicros = m_records[0]->getSize();

    // For each micro, calculate the sum of the records
    std::vector<uint64_t> l_sums(l_nbMicros, 0);
    for (uint8_t l_microIndex = 0; l_microIndex < l_nbMicros; l_microIndex++)
    {
        // Calculate the sum of the records
        uint64_t l_sum = 0;
        for (uint8_t l_recordIndex = 0; l_recordIndex < m_records.size(); l_recordIndex++)
        {
            l_sum += m_records[l_recordIndex]->getValue(l_microIndex);
        }
        l_sums[l_microIndex] = l_sum;
    }

    // Fill the matrix of impacts
    for (uint8_t l_impactorMicro = 0; l_impactorMicro < l_nbMicros; l_impactorMicro++)
    {
        for (uint8_t l_impactedMicro = 0; l_impactedMicro < l_nbMicros; l_impactedMicro++)
        {
            // Calculate the ratio of the sum of the records of the impacted micro
            // with the sum of the records of impactor micro
            coeff l_impact;
            if (l_sums[l_impactorMicro] != 0)
            {
                // Calculate the real impact
                l_impact.setFloat((float)l_sums[l_impactedMicro] / (float)l_sums[l_impactorMicro]);
            }
            else
            {
                // The sum of the impactor micro is 0
                // It seems to be an error but let's admit that the impact is 0
                printf("Error : the sum of the impactor micro is 0\n");
                l_impact.m_value = 0;
            }
            // Set the real impact
            impactsManager->setRealImpact(l_impactorMicro, l_impactedMicro, l_impact);
        }
    }
}

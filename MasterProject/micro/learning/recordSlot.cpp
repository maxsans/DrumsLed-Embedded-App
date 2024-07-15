#include "recordSlot.h"
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

void recordSlot::calculateImpacts(impactsManager *impactsManager, uint32_t mainMicroIndex)
{
    // Calculate the real impacts of the main micro of this record on all others micros
    // Method :
    // Calculate the average ratio of this micro on all others micros

    assert(m_records.size() > 0);
    assert( (mainMicroIndex < m_records.size()) && (mainMicroIndex >= 0) );

    uint32_t l_nbMicros = m_records[mainMicroIndex]->getSize();

    for (uint32_t l_ImpactedMicro = 0; l_ImpactedMicro < l_nbMicros; l_ImpactedMicro++)
    {
        if (l_ImpactedMicro == mainMicroIndex)
        {
            impactsManager->setRealImpact(mainMicroIndex, l_ImpactedMicro, 1.0f);
        }
        else
        {
            float l_sum = 0;
            uint32_t l_nbRecords = m_records.size();
            for (uint32_t l_recordIndex = 0; l_recordIndex < l_nbRecords; l_recordIndex++)
            {
                record *l_record = m_records[l_recordIndex];
                uint8_t l_ImpactorMicroValue = l_record->getValue(mainMicroIndex);
                uint8_t l_ImpactedMicroValue = l_record->getValue(l_ImpactedMicro);
                if (l_ImpactorMicroValue != 0)
                {
                    l_sum += (float)l_ImpactedMicroValue / (float)l_ImpactorMicroValue;
                }
            }
            float l_impact = l_sum / l_nbRecords;
            impactsManager->setRealImpact(mainMicroIndex, l_ImpactedMicro, l_impact);
        }
    }
}

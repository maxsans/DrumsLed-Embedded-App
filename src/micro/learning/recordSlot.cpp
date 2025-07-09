#include "recordSlot.h"
#include "impactsManager.h"

#include <assert.h>

RecordSlot::RecordSlot(uint32_t nbMicros)
{
    m_records.resize(nbMicros);
    for (uint32_t i = 0; i < nbMicros; i++)
    {
        m_records[i] = new Record(nbMicros);
    }
}

RecordSlot::~RecordSlot()
{
    for (uint32_t i = 0; i < m_records.size(); i++)
    {
        delete m_records[i];
    }
    m_records.clear();
}

void RecordSlot::addRecord(Record newRecord)
{
    // Control that the record has a good size
    // (all records must have the same size = number of micros)
    if (m_records.size() > 0)
    {
        assert(newRecord.getSize() == m_records[0]->getSize());
    }
    m_records.push_back(new Record(newRecord));
}

Record RecordSlot::getRecord(uint32_t index)
{
    assert(index < m_records.size());
    return *m_records[index];
}

uint32_t RecordSlot::getSize()
{
    return m_records.size();
}

void RecordSlot::calculateImpacts(ImpactsManager *impactsManager, uint32_t mainMicroIndex)
{
    // Calculate the real impacts of the main micro of this record on all others micros
    // Method :
    // Calculate the average ratio of this micro on all others micros

    Micro *l_mainMicro = impactsManager->getMicro(mainMicroIndex);

    assert(m_records.size() > 0);
    assert( (mainMicroIndex < m_records.size()) && (mainMicroIndex >= 0) );

    uint32_t l_nbMicros = m_records[mainMicroIndex]->getSize();

    for (uint32_t l_ImpactedMicroIndex = 0; l_ImpactedMicroIndex < l_nbMicros; l_ImpactedMicroIndex++)
    {
        Micro *l_ImpactedMicro = impactsManager->getMicro(l_ImpactedMicroIndex);
        if (l_ImpactedMicroIndex == mainMicroIndex)
        {
            impactsManager->setRealImpact(l_mainMicro, l_ImpactedMicro, 1.0f);
        }
        else
        {
            float l_sum = 0;
            uint32_t l_nbRecords = m_records.size();
            for (uint32_t l_recordIndex = 0; l_recordIndex < l_nbRecords; l_recordIndex++)
            {
                Record *l_record = m_records[l_recordIndex];
                uint8_t l_ImpactorMicroValue = l_record->getValue(mainMicroIndex);
                uint8_t l_ImpactedMicroValue = l_record->getValue(l_ImpactedMicroIndex);
                if (l_ImpactorMicroValue != 0)
                {
                    l_sum += (float)l_ImpactedMicroValue / (float)l_ImpactorMicroValue;
                }
            }
            float l_impact = l_sum / l_nbRecords;
            impactsManager->setRealImpact(l_mainMicro, l_ImpactedMicro, l_impact);
        }
    }
}

#include "record.h"

#include <assert.h>

record::record(uint32_t nbMicros)
{
    m_record.resize(nbMicros);
}

void record::setValue(uint32_t index, uint8_t value)
{
    assert(index < m_record.size());
    m_record.at(index) = value;
}

uint8_t record::getValue(uint32_t index)
{
    assert(index < m_record.size());
    return m_record[index];
}

uint32_t record::getSize()
{
    return m_record.size();
}

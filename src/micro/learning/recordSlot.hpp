/**
 * @file recordSlot.hpp
 * @brief Container for microphone measurement records during learning phase
 * @author Cyprien Ménard
 * @date 2025-08-20
 */

#ifndef __RECORD_SLOT_HPP__
#define __RECORD_SLOT_HPP__

#include "impactsManager.hpp"
#include "record.hpp"

#include <stdint.h>
#include <vector>

/**
 * @brief Container for storing and processing microphone measurement records for impact analysis
 */
class RecordSlot
{
    private:
    std::vector<Record *> m_records;

    public:
    RecordSlot(uint32_t nbMicros);
    ~RecordSlot();

    /**
     * @brief Add a record
     *
     * @param record The record to add
     */
    void addRecord(Record newRecord);

    /**
     * @brief Get a record
     *
     * @param index The index of the record
     * @return record The record
     */
    Record getRecord(uint32_t index);

    /**
     * @brief Get the number of records
     *
     * @return uint32_t The number of records
     */
    uint32_t getSize();

    /**
     * @brief Calculate the impacts of the micros on this record slot
     *
     * @param impactsManager The impacts manager to fill
     * @param mainMicroIndex The index of the main micro
     */
    void calculateImpacts(ImpactsManager *impactsManager,
                          uint32_t mainMicroIndex);
};

#endif // __RECORD_SLOT_HPP__

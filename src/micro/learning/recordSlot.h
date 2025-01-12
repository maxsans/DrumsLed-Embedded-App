#ifndef __RECORD_SLOT_H__
#define __RECORD_SLOT_H__

#include "record.h"
#include "impactsManager.h"

#include <stdint.h>
#include <vector>

/**
 * @brief A record slot is the result of a learning on a instrument
 * (a list of records so)
 *
 */
class recordSlot
{
    private:
        std::vector<record *> m_records;

    public:
        recordSlot(uint32_t nbMicros);
        ~recordSlot();

        /**
         * @brief Add a record
         *
         * @param record The record to add
         */
        void addRecord(record newRecord);

        /**
         * @brief Get a record
         *
         * @param index The index of the record
         * @return record The record
         */
        record getRecord(uint32_t index);

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
        void calculateImpacts(impactsManager *impactsManager, uint32_t mainMicroIndex);
};

#endif

#ifndef __RECORD_H__
#define __RECORD_H__

#include <vector>
#include <stdint.h>

/**
 * @brief A record is a slot of all micros values at a time
 *
 */
class record
{
    private:
        std::vector<uint8_t> m_record;

    public:
        record(uint32_t nbMicros);

        /**
         * @brief Set a value to the record
         *
         * @param index the index of the micro to set
         * @param value the value to add
         */
        void setValue(uint32_t index, uint8_t value);

        /**
         * @brief Get the value at a specific index
         *
         * @param index the index of the micro to get
         * @return the value of the micro
         */
        uint8_t getValue(uint32_t index);

        /**
         * @brief Get the size of the record
         *
         * @return the size of the record
         */
        uint32_t getSize();
};

#endif

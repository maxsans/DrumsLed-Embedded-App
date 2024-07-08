#ifndef __LEARNING_H__
#define __LEARNING_H__

#include "microManager.h"

#include <stdint.h>
#include <vector>

class learning
{
    private:
        microManager *m_manager;
        bool m_inLearning;
        int32_t m_MicroInRecord;
        /**
         * @brief Vector of records for each micro
         * First index is the micro index
         * Second index is the record index
         *
         */
        std::vector<std::vector<uint8_t>> m_microRecords;
        /**
         * @brief Record all the micros
         *
         */
        void record();
        /**
         * @brief Interprete the records
         *
         */
        void calculate();

    public:
        learning();
        learning(microManager *manager);
        ~learning();

        /**
         * @brief Set the manager
         *
         * @param manager The manager to set
         */
        void setManager(microManager *manager);

        /**
         * @brief Start the learning process on all the micros
         *
         */
        void startLearning();

        /**
         * @brief Start the learning process on a specific micro
         *
         * @param microIndex The index of the micro to start learning
         */
        void startLearning(int32_t microIndex);

        /**
         * @brief Function to process the learning
         * Must be called in the main loop
         *
         */
        void process();

        /**
         * @brief Stop the learning process
         *
         */
        void stopLearning();

};

#endif

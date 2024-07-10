#ifndef __LEARNING_H__
#define __LEARNING_H__

#include "microManager.h"
#include "ledManager.h"
#include "moduleManager.h"

#include <stdint.h>
#include <vector>

class learning
{
    private:
        microManager *m_microManager;
        ledManager *m_ledManager;
        moduleManager *m_moduleManager;
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
         * @brief Calculate the correction for last record
         *
         */
        void calculateCorrection();
        /**
         * @brief Calculate the real impact of each micro for last record
         *
         */
        void calculateRealImpacts();

    public:
        learning();
        ~learning();

        /**
         * @brief Check if a learning process is running
         *
         * @return true if a learning process is running
         */
        bool isLearning();

        /**
         * @brief Set the session to learn
         *
         * @param sessionToLearn The session to learn
         */
        void setSession(moduleManager *moduleManager, microManager *microManager, ledManager *ledManager);

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

        /**
         * @brief Print the results of the learning
         *
         */
        void printResults();
};

#endif

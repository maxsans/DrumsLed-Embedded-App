/*
    TODO: A lot of things must be refactored in this file !!!
*/

#ifndef __LEARNING_H__
#define __LEARNING_H__

#include "modules/moduleManager.h"
#include "recordSlot.h"

#include <stdint.h>
#include <vector>

class Learning
{
    private:
        ModuleManager *m_moduleManager;
        int32_t m_MicroInRecord;
        periodicCallsMs m_recordPeriodicCall;

        /**
         * @brief Vector of record slot for each micro
         *
         */
        std::vector<RecordSlot *> m_microRecordSlots;

        /**
         * @brief Callback to periodically call recordAllMic()
         *
         * @param object learning object (this)
         */
        static void recordCallback(void *object);

        /**
         * @brief Record all the micros
         *
         */
        void recordAllMic();

        /**
         * @brief Calculate the correction all the records
         *
         */
        void calculateCorrection();

        /**
         * @brief Calculate the real impact of each micro for all the records
         *
         */
        void calculateRealImpacts();

        /**
         * @brief Calculate the threshold of each micro
         *
         */
        void calculateThreshold();


    public:
        Learning(ModuleManager *moduleManager);
        ~Learning();

        /**
         * @brief Check if a learning process is running
         *
         * @return true if a learning process is running
         */
        bool isLearning();

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

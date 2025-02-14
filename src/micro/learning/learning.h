#ifndef __LEARNING_H__
#define __LEARNING_H__

#include "modules/moduleManager.h"
#include "recordSlot.h"

#include <stdint.h>
#include <vector>

class Learning
{
    private:
        static int32_t m_MicroInRecord;
        static periodicCallsMs m_recordPeriodicCall;

        /**
         * @brief Vector of record slot for each micro
         *
         */
        static std::vector<RecordSlot *> m_microRecordSlots;

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
        static void recordAllMic();

        /**
         * @brief Calculate the correction all the records
         *
         */
        static void calculateCorrection();

        /**
         * @brief Calculate the real impact of each micro for all the records
         *
         */
        static void calculateRealImpacts();

        /**
         * @brief Calculate the threshold of each micro
         *
         */
        static void calculateThreshold();

    public:
        Learning() = delete;

        /**
         * @brief Check if a learning process is running
         *
         * @return true if a learning process is running
         */
        static bool isLearning();

        /**
         * @brief Start the learning process on all the micros
         *
         */
        static void startLearning();

        /**
         * @brief Start the learning process on a specific micro
         *
         * @param microIndex The index of the micro to start learning
         */
        static void startLearning(int32_t microIndex);

        /**
         * @brief Stop the learning process
         *
         */
        static void stopLearning();

        /**
         * @brief Print the results of the learning
         *
         */
        static void printResults();
};

#endif

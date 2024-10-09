#ifndef __LEARNING_H__
#define __LEARNING_H__

#include "microManager.h"
#include "ledManager.h"
#include "moduleManager.h"
#include "recordSlot.h"

#include <stdint.h>
#include <vector>

class learning
{
    private:
        microManager *m_microManager;
        ledManager *m_ledManager;
        moduleManager *m_moduleManager;
        int32_t m_MicroInRecord;
        periodicCallsMs m_recordPeriodicCall;
        /**
         * @brief Vector of record slot for each micro
         *
         */
        std::vector<recordSlot *> m_microRecordSlots;
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

#ifndef __LEARNING_HPP__
#define __LEARNING_HPP__

#include "impactsManager.hpp"
#include "modules/moduleManager.hpp"
#include "recordSlot.hpp"

#include <stdint.h>
#include <vector>

class Learning
{
    private:
    /**
     * @brief Maximum number of measures to record
     * @note At the maximum, there is a measure every m_timeBetweenMeasures ms
     */
    static constexpr uint32_t m_nbMesuresMax = 200;

    /**
     * @brief Time between each measure
     * @note Unit : ms
     */
    static const timeMs m_timeBetweenMeasures;

    /**
     * @brief Threshold to consider that a micro has recorded something
     */
    static constexpr uint8_t m_recordLimit = 10;

    /**
     * @brief Normalization value for the correction
     * @note The maximum of each micro on his learning must be normalized at this value
     */
    static constexpr uint8_t m_correctionNormalization = 180;

    /**
     * @brief Margin to add to the threshold
     */
    static constexpr uint8_t m_thresholdMargin = 0;

    /**
     * @brief Color of the led when a module is in learning
     */
    static const RgbColor m_learningColor;

    /**
     * @brief Color of the led when a module is not in learning
     */
    static const RgbColor m_notLearningColor;

    /**
     * @brief Index of the micro currently being recorded
     * @note This is used to know which micro is currently being recorded.
     */
    int32_t m_MicroInRecord;

    /**
     * @brief Periodic call to record all the micros
     * This periodic call is used to record all the micros at a regular interval.
     */
    periodicCallsMs m_recordPeriodicCall;

    /**
     * @brief Module manager on which the learning is done
     */
    ModuleManager *m_moduleManager;

    /**
     * @brief Vector of record slot for each micro
     */
    std::vector<RecordSlot *> m_microRecordSlots;

    /**
     * @brief Callback to call when the learning is done
     */
    void (*m_learningDoneCallback)(void *object);
    void *m_learningDoneCallbackObject;

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
    /**
     * @brief Initialize the learning process
     * @param modules The vector of modules to learn
     * @param learningDoneCallback The callback to call when the learning is done
     */
    Learning(ModuleManager *moduleManager,
             void (*learningDoneCallback)(void *object) = nullptr,
             void *obj = nullptr);

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
     * @return true if it was the last micro to learn, false if there are still micros to learn
     */
    bool stopLearning();

    /**
     * @brief Print the results of the learning
     *
     */
    void printResults();
};

#endif

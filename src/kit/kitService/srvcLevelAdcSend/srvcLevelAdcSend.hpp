/**
 * @file srvcLevelAdcSend.hpp
 * @brief Kit service for ADC level measurement and transmission to master
 * @author Cyprien Ménard
 * @date 2025-08-20
 */

#ifndef __SRVC_LEVEL_ADC_SEND_HPP__
#define __SRVC_LEVEL_ADC_SEND_HPP__

#include "api/adc/adc.hpp"
#include "kit/kitService/kitService.hpp"
#include "tools/os/periodicCallsMs/periodicCallsMs.hpp"

/**
 * @brief Structure to hold ADC measurement with timestamp
 */
struct AdcMeasurement
{
    adc_measure_t value;
    TimeMs timestamp;
};

/**
 * @brief Service for measuring ADC levels with circular buffering and sending averaged data to master
 */
class KitSrvcLevelAdcSend : public KitService
{
    private:
    /**
     * @brief The interval in milliseconds for sending ADC level data.
     */
    static const TimeMs m_sendInterval;

    /**
     * @brief The interval in milliseconds for measuring ADC level data.
     */
    static const TimeMs m_measureInterval;

    /**
     * @brief The time in milliseconds for the circular buffer to hold measurements.
     * @note This is used to determine how long the buffer should keep the measurements.
     */
    static const TimeMs m_bufferTime;

    /**
     * @brief Maximum buffer size to accommodate high-frequency measurements.
     */
    static const uint32_t m_maxBufferSize;

    /**
     * @brief The periodic call for sending ADC level data.
     */
    PeriodicCallsMs m_sendPeriodicCall;

    /**
     * @brief The periodic call for measuring ADC level data.
     * @note This is used to measure the ADC level data periodically.
     */
    PeriodicCallsMs m_measurePeriodicCall;

    /**
     * @brief Current index in the circular buffer.
     */
    uint32_t m_currentIndex;

    /**
     * @brief Current size of the circular buffer.
     * @note This is useful when the buffer is not full yet.
     */
    uint32_t m_currentSize;

    /**
     * @brief Circular buffer to store ADC measurements with timestamps.
     * @note This buffer will hold the last measurements within m_bufferTime.
     */
    AdcMeasurement *m_circularBuffer;

    /**
     * @brief Sum of all measurements in the circular buffer.
     * @note This is used to calculate the average ADC level.
     */
    uint32_t m_bufferSum;

    /**
     * @brief Get the number of measurements in the circular buffer.
     */
    uint32_t getBufferSize();

    /**
     * @brief Get the average ADC level from the circular buffer.
     * @return The average ADC level.
     */
    adc_measure_t getAverageAdcLevel();

    /**
     * @brief Callback function for periodic sending of ADC level data.
     */
    void periodicSendCallback();

    /**
     * @brief Callback function for periodic measuring of ADC level data.
     */
    void periodicMeasureCallback();

    /**
     * @brief Measure the ADC level.
     * @return The measured ADC level.
     */
    adc_measure_t measureAdcLevel();

    /**
     * @brief Get the number of valid measurements in the circular buffer within the time window.
     */
    uint32_t getValidMeasurementCount();

    /**
     * @see KitService::onStart
     */
    void onStart() override;

    /**
     * @see KitService::onStop
     */
    void onStop() override;

    public:
    /**
     * @brief Constructor for KitSrvcLevelAdcSend.
     */
    KitSrvcLevelAdcSend();
};

#endif // __SRVC_LEVEL_ADC_SEND_HPP__

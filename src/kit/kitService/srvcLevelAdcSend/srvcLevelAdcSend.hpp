#ifndef __SRVC_LEVEL_ADC_SEND_HPP__
#define __SRVC_LEVEL_ADC_SEND_HPP__

#include "kit/kitService/kitService.hpp"
#include "tools/timeTools/periodicCallsMs.hpp"
#include "api/adc/adc.hpp"

class KitSrvcLevelAdcSend : public KitService
{
    private:
        /**
         * @brief The interval in milliseconds for sending ADC level data.
         */
        static const timeMs m_sendInterval;

        /**
         * @brief The interval in milliseconds for measuring ADC level data.
         */
        static const timeMs m_measureInterval;

        /**
         * @brief The time in milliseconds for the circular buffer to hold measurements.
         * @note This is used to determine how long the buffer should keep the measurements.
         */
        static const timeMs m_bufferTime;

        /**
         * @brief The periodic call for sending ADC level data.
         */
        periodicCallsMs m_sendPeriodicCall;

        /**
         * @brief The periodic call for measuring ADC level data.
         * @note This is used to measure the ADC level data periodically.
         */
        periodicCallsMs m_measurePeriodicCall;

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
         * @brief Circular buffer to store ADC measurements.
         * @note This buffer will hold the last KitSrvcLevelAdcSend::getBufferSize() measurements.
         */
        adc_measure_t *m_circularBuffer;

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
        static void periodicSendCallback(void *object);
        void periodicSendCallback();

        /**
         * @brief Callback function for periodic measuring of ADC level data.
         */
        static void periodicMeasureCallback(void *object);
        void periodicMeasureCallback();

        /**
         * @brief Measure the ADC level.
         * @return The measured ADC level.
         */
        adc_measure_t measureAdcLevel();

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

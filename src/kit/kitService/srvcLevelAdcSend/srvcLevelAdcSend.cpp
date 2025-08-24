/**
 * @file srvcLevelAdcSend.cpp
 * @author Cyprien Ménard
 * @date 2025-08-20
 */

#include "srvcLevelAdcSend.hpp"
#include "network/interCom/interMsg/interMsgList/interMsgAdc/interMsgAdc.hpp"
#include <cstring>

const TimeMs KitSrvcLevelAdcSend::m_sendInterval = TimeMs(20);
const TimeMs KitSrvcLevelAdcSend::m_measureInterval = TimeMs(0);
const TimeMs KitSrvcLevelAdcSend::m_bufferTime = TimeMs(100);
const uint32_t KitSrvcLevelAdcSend::m_maxBufferSize
    = 1000; // Large buffer for high-frequency measurements

KitSrvcLevelAdcSend::KitSrvcLevelAdcSend()
    : KitService(KitServiceType::LevelAdcSend),
      m_sendPeriodicCall(m_sendInterval,
                         [this]() { this->periodicSendCallback(); }),
      m_measurePeriodicCall(m_measureInterval,
                            [this]() { this->periodicMeasureCallback(); }),
      m_currentIndex(0), m_currentSize(0),
      m_circularBuffer(new AdcMeasurement[m_maxBufferSize]), m_bufferSum(0)
{
    // Initialize the circular buffer properly
    for (uint32_t i = 0; i < m_maxBufferSize; i++)
    {
        m_circularBuffer[i].value = 0;
        m_circularBuffer[i].timestamp = TimeMs(0);
    }

    // For now the communication isn't established with the master client
    m_sendPeriodicCall.enable(false);
}

uint32_t KitSrvcLevelAdcSend::getBufferSize()
{
    return m_maxBufferSize;
}

uint32_t KitSrvcLevelAdcSend::getValidMeasurementCount()
{
    TimeMs currentTime = TimeMs::nowMs();
    uint32_t validCount = 0;

    for (uint32_t i = 0; i < m_currentSize; i++)
    {
        if ((currentTime - m_circularBuffer[i].timestamp) <= m_bufferTime)
        {
            validCount++;
        }
    }

    return validCount;
}

adc_measure_t KitSrvcLevelAdcSend::getAverageAdcLevel()
{
    TimeMs currentTime = TimeMs::nowMs();
    uint32_t validSum = 0;
    uint32_t validCount = 0;

    // Calculate average only from measurements within the time window
    for (uint32_t i = 0; i < m_currentSize; i++)
    {
        if ((currentTime - m_circularBuffer[i].timestamp) <= m_bufferTime)
        {
            validSum += m_circularBuffer[i].value;
            validCount++;
        }
    }

    if (validCount == 0)
    {
        return 0; // Avoid division by zero
    }
    return validSum / validCount;
}

void KitSrvcLevelAdcSend::periodicSendCallback()
{
    adc_measure_t averageLevel = getAverageAdcLevel();
    Client l_masterClient = getMasterClient();
    InterMsgAdc l_msgAdc(l_masterClient, averageLevel);
    // Send the ADC level message to the master client
    l_msgAdc.send();
}

void KitSrvcLevelAdcSend::periodicMeasureCallback()
{
    adc_measure_t level = measureAdcLevel();
    TimeMs currentTime = TimeMs::nowMs();

    // Add the new measurement with timestamp
    m_circularBuffer[m_currentIndex].value = level;
    m_circularBuffer[m_currentIndex].timestamp = currentTime;

    // Update the current index and size of the buffer
    m_currentIndex = (m_currentIndex + 1) % m_maxBufferSize;
    if (m_currentSize < m_maxBufferSize)
    {
        m_currentSize++;
    }
}

adc_measure_t KitSrvcLevelAdcSend::measureAdcLevel()
{
    static bool adc_initialized = false;
    if (!adc_initialized)
    {
        adc_init();
        adc_initialized = true;
    }
    // Read the ADC value using the adc API
    return adc_read();
}

void KitSrvcLevelAdcSend::onStart()
{
    // ADC initialization is handled in measureAdcLevel() to avoid multiple initializations
    // Start the periodic calls for sending ADC level data
    // since the communication with the master client is established
    m_sendPeriodicCall.enable(true);
}

void KitSrvcLevelAdcSend::onStop()
{
    // Stop the periodic calls for sending ADC level data
    m_sendPeriodicCall.enable(false);
    // Clean up the circular buffer properly
    for (uint32_t i = 0; i < m_maxBufferSize; i++)
    {
        m_circularBuffer[i].value = 0;
        m_circularBuffer[i].timestamp = TimeMs(0);
    }
    m_currentIndex = 0;
    m_currentSize = 0;
    m_bufferSum = 0;
}

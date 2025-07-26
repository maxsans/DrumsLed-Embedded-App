#include "srvcLevelAdcSend.hpp"
#include "network/interCom/interMsgList/interMsgAdc/interMsgAdc.hpp"
#include <cstring>

const timeMs KitSrvcLevelAdcSend::m_sendInterval = timeMs(20);
const timeMs KitSrvcLevelAdcSend::m_measureInterval = timeMs(5);
const timeMs KitSrvcLevelAdcSend::m_bufferTime = timeMs(100);

KitSrvcLevelAdcSend::KitSrvcLevelAdcSend()
    : KitService(KitServiceType::LevelAdcSend),
      m_sendPeriodicCall(m_sendInterval, &KitSrvcLevelAdcSend::periodicSendCallback, this),
      m_measurePeriodicCall(m_measureInterval, &KitSrvcLevelAdcSend::periodicMeasureCallback, this),
      m_currentIndex(0),
      m_currentSize(0),
      m_circularBuffer(new adc_measure_t[getBufferSize()]),
      m_bufferSum(0)
{
    // For now the communication isn't established with the master client
    m_sendPeriodicCall.enable(false);
}

uint32_t KitSrvcLevelAdcSend::getBufferSize()
{
    // Calculate the buffer size based on the buffer time and measurement interval
    return (uint32_t)(m_bufferTime.get()) / (uint32_t)(m_measureInterval.get());
}

adc_measure_t KitSrvcLevelAdcSend::getAverageAdcLevel()
{
    if (m_currentSize == 0)
    {
        return 0; // Avoid division by zero
    }
    return m_bufferSum / m_currentSize;
}

void KitSrvcLevelAdcSend::periodicSendCallback(void *object)
{
    KitSrvcLevelAdcSend *service = static_cast<KitSrvcLevelAdcSend *>(object);
    service->periodicSendCallback();
}

void KitSrvcLevelAdcSend::periodicSendCallback()
{
    adc_measure_t averageLevel = getAverageAdcLevel();
    Client l_masterClient = getMasterClient();
    InterMsgAdc l_msgAdc(l_masterClient, averageLevel);
    // Send the ADC level message to the master client
    l_msgAdc.send();
}

void KitSrvcLevelAdcSend::periodicMeasureCallback(void *object)
{
    KitSrvcLevelAdcSend *service = static_cast<KitSrvcLevelAdcSend *>(object);
    service->periodicMeasureCallback();
}

void KitSrvcLevelAdcSend::periodicMeasureCallback()
{
    adc_measure_t level = measureAdcLevel();

    // Update the circular buffer with the new measurement
    m_bufferSum -= m_circularBuffer[m_currentIndex]; // Remove the oldest measurement from the sum
    m_circularBuffer[m_currentIndex] = level; // Add the new measurement
    m_bufferSum += level; // Update the sum with the new measurement

    // Update the current index and size of the buffer
    m_currentIndex = (m_currentIndex + 1) % getBufferSize();
    if (m_currentSize < getBufferSize())
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
    // Init the ADC
    adc_init();
    // Start the periodic calls for sending ADC level data
    // since the communication with the master client is established
    m_sendPeriodicCall.enable(true);
}

void KitSrvcLevelAdcSend::onStop()
{
    // Stop the periodic calls for sending ADC level data
    m_sendPeriodicCall.enable(false);
    // Clean up the circular buffer
    memset(m_circularBuffer, 0, getBufferSize() * sizeof(adc_measure_t));
    m_currentIndex = 0;
    m_currentSize = 0;
    m_bufferSum = 0;
}

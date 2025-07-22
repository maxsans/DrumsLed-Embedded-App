#include "srvcPeriodicLevelAdcSend.hpp"
#include "network/interCom/interMsgList/interMsgAdc/interMsgAdc.hpp"

const timeMs KitServicePeriodicLevelAdcSend::m_sendInterval = timeMs(20);
const timeMs KitServicePeriodicLevelAdcSend::m_measureInterval = timeMs(1);
const timeMs KitServicePeriodicLevelAdcSend::m_bufferTime = timeMs(100);

KitServicePeriodicLevelAdcSend::KitServicePeriodicLevelAdcSend()
    : KitService(KitServiceType::LevelAdcSend),
      m_sendPeriodicCall(m_sendInterval, &KitServicePeriodicLevelAdcSend::periodicSendCallback, this),
      m_measurePeriodicCall(m_measureInterval, &KitServicePeriodicLevelAdcSend::periodicMeasureCallback, this),
      m_currentIndex(0),
      m_currentSize(0),
      m_circularBuffer(new adc_measure_t[getBufferSize()]),
      m_bufferSum(0)
{
    // For now the communication isn't established with the master client
    m_sendPeriodicCall.enable(false);
}

uint32_t KitServicePeriodicLevelAdcSend::getBufferSize()
{
    // Calculate the buffer size based on the buffer time and measurement interval
    return (uint32_t)(m_bufferTime.get()) / (uint32_t)(m_measureInterval.get());
}

adc_measure_t KitServicePeriodicLevelAdcSend::getAverageAdcLevel()
{
    if (m_currentSize == 0)
    {
        return 0; // Avoid division by zero
    }
    return m_bufferSum / m_currentSize;
}

void KitServicePeriodicLevelAdcSend::periodicSendCallback(void *object)
{
    KitServicePeriodicLevelAdcSend *service = static_cast<KitServicePeriodicLevelAdcSend *>(object);
    service->periodicSendCallback();
}

void KitServicePeriodicLevelAdcSend::periodicSendCallback()
{
    adc_measure_t averageLevel = getAverageAdcLevel();
    Client l_masterClient = getMasterClient();
    InterMsgAdc l_msgAdc(l_masterClient, averageLevel);
    // Send the ADC level message to the master client
    l_msgAdc.send();
}

void KitServicePeriodicLevelAdcSend::periodicMeasureCallback(void *object)
{
    KitServicePeriodicLevelAdcSend *service = static_cast<KitServicePeriodicLevelAdcSend *>(object);
    service->periodicMeasureCallback();
}

void KitServicePeriodicLevelAdcSend::periodicMeasureCallback()
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

adc_measure_t KitServicePeriodicLevelAdcSend::measureAdcLevel()
{
    // Read the ADC value using the adc API
    return adc_read();
}

void KitServicePeriodicLevelAdcSend::onStart()
{
    // Init the ADC
    adc_init();
    // Start the periodic calls for sending ADC level data
    // since the communication with the master client is established
    m_sendPeriodicCall.enable(true);
}

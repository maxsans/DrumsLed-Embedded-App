#include "mutex.hpp"
#include <freertos/FreeRTOS.h>
#include <freertos/semphr.h>

Mutex::Mutex()
{
    m_privateData = xSemaphoreCreateMutex();
}

Mutex::~Mutex()
{
    if (m_privateData)
    {
        vSemaphoreDelete(static_cast<SemaphoreHandle_t>(m_privateData));
    }
}

void Mutex::lock()
{
    if (m_privateData)
    {
        xSemaphoreTake(static_cast<SemaphoreHandle_t>(m_privateData),
                       portMAX_DELAY);
    }
}

void Mutex::unlock()
{
    if (m_privateData)
    {
        xSemaphoreGive(static_cast<SemaphoreHandle_t>(m_privateData));
    }
}

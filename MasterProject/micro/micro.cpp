#include "micro.h"

#include <stdio.h>
#include <windows.h>

#define MICRO_TIMEOUT 100 // ms
#define DEFAULT_CORRECTION 1.00
#define DEFAULT_THRESHOLD 5

micro::micro(module *m)
{
    // Constructor
    m_connected = true;
    m_module = m;
    m_microValue = 0;
    m_microValueCorrected = 0;
    m_correction = DEFAULT_CORRECTION;
    m_threshold = DEFAULT_THRESHOLD;
    m_lastSyncTime = GetTickCount64();
}

void micro::process()
{
    // Process the micro
    // Check if the micro is still connected
    checkTime();
}

void micro::sync()
{
    // Sync the micro
    if (!m_connected)
    {
        // printf("Micro reconnected, ip: %s\n", m_module->getIp());
        m_connected = true;
    }
    m_lastSyncTime = GetTickCount64();
}

void micro::checkTime()
{
    // Check if the micro is still connected
    if (GetTickCount64() - m_lastSyncTime > MICRO_TIMEOUT)
    {
        if (m_connected)
        {
            // printf("Micro disconnected, ip: %s\n", m_module->getIp());
            m_connected = false;
            // Reset values to 0
            m_microValue = 0;
            m_microValueCorrected = 0;
        }
    }
}

bool micro::isConnected()
{
    // Check if the micro is connected
    return m_connected;
}

module *micro::getModule()
{
    // Get the module of the micro
    return m_module;
}

void micro::setCorrection(float correction)
{
    // Set the correction of the micro
    m_correction = correction;
}

float micro::getCorrection()
{
    // Get the correction of the micro
    return m_correction;
}

void micro::setMicroValue(uint8_t microValue)
{
    // Sync the micro
    sync();
    // Set the value of the micro
    m_microValue = microValue;
}

void micro::setMicroValueCorrected(uint8_t microValueCorrected)
{
    // Set the corrected value of the micro
    m_microValueCorrected = microValueCorrected;
}

void micro::setThreshold(uint8_t threshold)
{
    // Set the threshold of the micro
    m_threshold = threshold;
}

uint8_t micro::getMicroValue()
{
    // Get the value of the micro
    return m_microValue;
}

uint8_t micro::getMicroValueCorrected()
{
    // Get the corrected value of the micro
    return m_microValueCorrected;
}

uint8_t micro::getThreshold()
{
    // Get the threshold of the micro
    return m_threshold;
}

bool micro::isHit()
{
    // Check if the micro is hit
    return (m_microValueCorrected > (m_threshold*m_correction));
}

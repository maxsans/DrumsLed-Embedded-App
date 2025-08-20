/**
 * @file micro.cpp
 * @author Cyprien Ménard
 * @date 2025-08-20
 */

#include "micro.hpp"
#include "api/time/time.hpp"
#include "tools/logStream/logStream.hpp"

#define MICRO_TIMEOUT 100 // ms
#define DEFAULT_CORRECTION 1.00
#define DEFAULT_THRESHOLD 5

Micro::Micro()
{
    // Constructor
    m_connected = true;
    m_microValue = 0;
    m_microValueCorrected = 0;
    m_correction = DEFAULT_CORRECTION;
    m_threshold = DEFAULT_THRESHOLD;
    m_lastSyncTime = TimeMs();
}

void Micro::process()
{
    // Process the micro
    // Check if the micro is still connected
    checkTime();
}

void Micro::sync()
{
    // Sync the micro
    if (!m_connected)
    {
        m_connected = true;
    }
    m_lastSyncTime = TimeMs();
}

void Micro::checkTime()
{
    // Check if the micro is still connected
    if (TimeMs() - m_lastSyncTime > MICRO_TIMEOUT)
    {
        if (m_connected)
        {
            m_connected = false;
            // Reset values to 0
            m_microValue = 0;
            m_microValueCorrected = 0;
        }
    }
}

bool Micro::isConnected()
{
    // Check if the micro is connected
    return m_connected;
}

void Micro::setCorrection(float correction)
{
    // Set the correction of the micro
    m_correction = correction;
}

float Micro::getCorrection()
{
    // Get the correction of the micro
    return m_correction;
}

void Micro::setMicroValue(uint8_t microValue)
{
    // Sync the micro
    sync();
    // Set the value of the micro
    m_microValue = microValue;
}

void Micro::setMicroValueCorrected(uint8_t microValueCorrected)
{
    // Apply the correction to the micro value
    m_microValueCorrected = microValueCorrected * m_correction;
}

void Micro::setThreshold(uint8_t threshold)
{
    // Set the threshold of the micro
    m_threshold = threshold;
}

uint8_t Micro::getMicroValue()
{
    // Get the value of the micro
    return m_microValue;
}

uint8_t Micro::getMicroValueCorrected()
{
    // Get the corrected value of the micro
    return m_microValueCorrected;
}

uint8_t Micro::getThreshold()
{
    // Get the threshold of the micro
    return m_threshold;
}

bool Micro::isHit()
{
    // Check if the micro is hit
    return (m_microValueCorrected > (m_threshold * m_correction));
}

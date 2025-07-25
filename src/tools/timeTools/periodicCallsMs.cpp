#include "periodicCallsMs.hpp"
#include <algorithm>

std::vector<periodicCallsMs*> periodicCallsMs::m_instances;

periodicCallsMs::periodicCallsMs()
{
    m_instances.push_back(this);
    m_enable = true;
}

periodicCallsMs::periodicCallsMs(timeMs period, void (*callback)(void*), void *object)
{
    m_instances.push_back(this);
    m_chrono.arm(period);
    m_callback = callback;
    m_object = object;
    m_enable = true;
}

periodicCallsMs::~periodicCallsMs()
{
    m_instances.erase(std::remove(m_instances.begin(), m_instances.end(), this), m_instances.end());
}

void periodicCallsMs::processAll()
{
    // Iterate through all instances and call their process method
    for (periodicCallsMs *instance : m_instances)
    {
        instance->process();
    }
}

void periodicCallsMs::process()
{
    if ( (m_chrono.ring()) && (m_enable))
    {
        m_callback(m_object);
        m_chrono.restart();
    }
}

void periodicCallsMs::setPeriod(timeMs period)
{
    m_chrono.arm(period);
}

void periodicCallsMs::setCallback(void (*callback)(void*), void *object)
{
    m_callback = callback;
    m_object = object;
}

void periodicCallsMs::enable(bool enable)
{
    m_enable = enable;
}

bool periodicCallsMs::isEnabled()
{
    return m_enable;
}

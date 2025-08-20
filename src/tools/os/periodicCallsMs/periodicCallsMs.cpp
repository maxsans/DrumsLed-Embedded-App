/**
 * @file PeriodicCallsMs.cpp
 * @author Cyprien Ménard
 * @date 2025-08-20
 */

#include "periodicCallsMs.hpp"
#include <algorithm>

std::vector<PeriodicCallsMs *> PeriodicCallsMs::m_instances;

PeriodicCallsMs::PeriodicCallsMs()
{
    m_instances.push_back(this);
    m_enable = true;
}

PeriodicCallsMs::PeriodicCallsMs(TimeMs period,
                                 void (*callback)(void *),
                                 void *object)
{
    m_instances.push_back(this);
    m_chrono.arm(period);
    m_callback = callback;
    m_object = object;
    m_enable = true;
}

PeriodicCallsMs::~PeriodicCallsMs()
{
    m_instances.erase(std::remove(m_instances.begin(), m_instances.end(), this),
                      m_instances.end());
}

void PeriodicCallsMs::processAll()
{
    // Iterate through all instances and call their process method
    for (PeriodicCallsMs *instance : m_instances)
    {
        instance->process();
    }
}

void PeriodicCallsMs::process()
{
    if ((m_chrono.ring()) && (m_enable))
    {
        m_callback(m_object);
        m_chrono.restart();
    }
}

void PeriodicCallsMs::setPeriod(TimeMs period)
{
    m_chrono.arm(period);
}

void PeriodicCallsMs::setCallback(void (*callback)(void *), void *object)
{
    m_callback = callback;
    m_object = object;
}

void PeriodicCallsMs::enable(bool enable)
{
    m_enable = enable;
}

bool PeriodicCallsMs::isEnabled()
{
    return m_enable;
}

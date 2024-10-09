#include "periodicCallsMs.h"

std::list <periodicCallsMs*> periodicCallsMs::m_instances;

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
}

periodicCallsMs::~periodicCallsMs()
{
    m_instances.remove(this);
}

void periodicCallsMs::processAll()
{
    for (std::list <periodicCallsMs*>::iterator it = m_instances.begin(); it != m_instances.end(); it++)
    {
        (*it)->process();
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

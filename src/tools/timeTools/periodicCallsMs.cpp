#include "periodicCallsMs.hpp"

std::list<periodicCallsMs*>& periodicCallsMs::instances()
{
    static std::list<periodicCallsMs*> s_instances;
    return s_instances;
}

periodicCallsMs::periodicCallsMs()
{
    instances().push_back(this);
    m_enable = true;
}

periodicCallsMs::periodicCallsMs(timeMs period, void (*callback)(void*), void *object)
{
    instances().push_back(this);
    m_chrono.arm(period);
    m_callback = callback;
    m_object = object;
    m_enable = true;
}

periodicCallsMs::~periodicCallsMs()
{
    instances().remove(this);
}

void periodicCallsMs::processAll()
{
    for (std::list <periodicCallsMs*>::iterator it = instances().begin(); it != instances().end(); it++)
    {
        if (*it != nullptr)
        {
            (*it)->process();
        }
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

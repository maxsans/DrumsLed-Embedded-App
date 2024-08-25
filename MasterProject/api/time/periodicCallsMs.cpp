#include "periodicCallsMs.h"

std::list <periodicCallsMs*> periodicCallsMs::m_instances;

periodicCallsMs::periodicCallsMs()
{
    m_instances.push_back(this);
}

periodicCallsMs::periodicCallsMs(timeMs period, void (*callback)())
{
    m_instances.push_back(this);
    m_chrono.arm(period);
    m_callback = callback;
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
    if (m_chrono.ring())
    {
        m_callback();
        m_chrono.restart();
    }
}

void periodicCallsMs::setPeriod(timeMs period)
{
    m_chrono.arm(period);
}

void periodicCallsMs::setCallback(void (*callback)())
{
    m_callback = callback;
}

#include "chronoMs.hpp"

ChronoMs::ChronoMs()
{
    m_armed = false;
}

void ChronoMs::start()
{
    m_start.setNow();
    m_armed = false;
}

void ChronoMs::setStart(TimeMs start)
{
    m_start = start;
    m_armed = false;
}

void ChronoMs::arm(TimeMs duration)
{
    m_end = m_start + duration;
    m_armed = true;
}

bool ChronoMs::ring()
{
    if (m_armed)
    {
        return m_end.get() <= TimeMs::nowMs();
    }

    return false;
}

bool ChronoMs::oneTimeRing()
{
    if (m_armed)
    {
        if (m_end.get() <= TimeMs::nowMs())
        {
            m_armed = false;
            return true;
        }
    }

    return false;
}

void ChronoMs::restart()
{
    TimeMs l_duration = m_end - m_start;
    start();
    arm(l_duration);
}

TimeMs ChronoMs::getStart()
{
    return m_start;
}

TimeMs ChronoMs::getEnd()
{
    return m_end;
}

TimeMs ChronoMs::getElapsedTime()
{
    return TimeMs::nowMs() - m_start.get();
}

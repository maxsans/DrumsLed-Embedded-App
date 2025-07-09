#include "chronoMs.h"

chronoMs::chronoMs()
{
    m_armed = false;
}

void chronoMs::start()
{
    m_start.setNow();
    m_armed = false;
}

void chronoMs::setStart(timeMs start)
{
    m_start = start;
    m_armed = false;
}

void chronoMs::arm(timeMs duration)
{
    m_end = m_start + duration;
    m_armed = true;
}

bool chronoMs::ring()
{
    if (m_armed)
    {
        if (m_end.get() < timeMs::nowMs())
        {
            return true;
        }
    }

    return false;
}

bool chronoMs::oneTimeRing()
{
    if (m_armed)
    {
        if (m_end.get() < timeMs::nowMs())
        {
            m_armed = false;
            return true;
        }
    }

    return false;
}

void chronoMs::restart()
{
    timeMs l_duration = m_end - m_start;
    start();
    arm(l_duration);
}

timeMs chronoMs::getStart()
{
    return m_start;
}

timeMs chronoMs::getEnd()
{
    return m_end;
}

timeMs chronoMs::getElapsedTime()
{
    return timeMs::nowMs() - m_start.get();
}


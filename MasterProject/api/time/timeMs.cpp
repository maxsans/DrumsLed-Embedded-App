#include "timeMs.h"

#include <windows.h>

timeMs::timeMs()
{
    m_timeMs = 0;
}

timeMs::timeMs(uint64_t timeMs)
{
    m_timeMs = timeMs;
}

void timeMs::set(uint64_t timeMs)
{
    m_timeMs = timeMs;
}

void timeMs::setNow()
{
    m_timeMs = nowMs();
}

uint64_t timeMs::nowMs()
{
    return GetTickCount64();
}

uint64_t timeMs::get()
{
    return m_timeMs;
}

uint64_t timeMs::toSeconds()
{
    return m_timeMs / 1000;
}

uint64_t timeMs::toMinutes()
{
    return m_timeMs / 60000;
}

timeMs timeMs::operator+(const timeMs &time)
{
    return timeMs(m_timeMs + time.m_timeMs);
}

timeMs timeMs::operator-(const timeMs &time)
{
    return timeMs(m_timeMs - time.m_timeMs);
}

timeMs timeMs::operator*(const timeMs &time)
{
    return timeMs(m_timeMs * time.m_timeMs);
}

timeMs timeMs::operator/(const timeMs &time)
{
    return timeMs(m_timeMs / time.m_timeMs);
}

bool timeMs::operator==(const timeMs &time)
{
    return m_timeMs == time.m_timeMs;
}

bool timeMs::operator!=(const timeMs &time)
{
    return m_timeMs != time.m_timeMs;
}

bool timeMs::operator>(const timeMs &time)
{
    return m_timeMs > time.m_timeMs;
}

bool timeMs::operator<(const timeMs &time)
{
    return m_timeMs < time.m_timeMs;
}

bool timeMs::operator>=(const timeMs &time)
{
    return m_timeMs >= time.m_timeMs;
}

bool timeMs::operator<=(const timeMs &time)
{
    return m_timeMs <= time.m_timeMs;
}

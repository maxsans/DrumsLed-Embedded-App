#include "timeMs.hpp"

#include "api/time/time.hpp"

TimeMs::TimeMs()
{
    m_timeMs = 0;
}

TimeMs::TimeMs(time_ms_t TimeMs)
{
    m_timeMs = TimeMs;
}

void TimeMs::set(time_ms_t TimeMs)
{
    m_timeMs = TimeMs;
}

void TimeMs::setNow()
{
    m_timeMs = nowMs();
}

time_ms_t TimeMs::nowMs()
{
    return time_ms();
}

time_ms_t TimeMs::get() const
{
    return m_timeMs;
}

time_ms_t TimeMs::toSeconds()
{
    return m_timeMs / 1000;
}

time_ms_t TimeMs::toMinutes()
{
    return m_timeMs / 60000;
}

TimeMs TimeMs::operator+(const TimeMs &time)
{
    return TimeMs(m_timeMs + time.m_timeMs);
}

TimeMs TimeMs::operator-(const TimeMs &time)
{
    return TimeMs(m_timeMs - time.m_timeMs);
}

TimeMs TimeMs::operator*(const TimeMs &time)
{
    return TimeMs(m_timeMs * time.m_timeMs);
}

TimeMs TimeMs::operator/(const TimeMs &time)
{
    return TimeMs(m_timeMs / time.m_timeMs);
}

bool TimeMs::operator==(const TimeMs &time)
{
    return m_timeMs == time.m_timeMs;
}

bool TimeMs::operator!=(const TimeMs &time)
{
    return m_timeMs != time.m_timeMs;
}

bool TimeMs::operator>(const TimeMs &time)
{
    return m_timeMs > time.m_timeMs;
}

bool TimeMs::operator<(const TimeMs &time)
{
    return m_timeMs < time.m_timeMs;
}

bool TimeMs::operator>=(const TimeMs &time)
{
    return m_timeMs >= time.m_timeMs;
}

bool TimeMs::operator<=(const TimeMs &time)
{
    return m_timeMs <= time.m_timeMs;
}

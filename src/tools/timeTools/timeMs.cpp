#include "timeMs

#include "api/time/time

timeMs::timeMs()
{
    m_timeMs = 0;
}

timeMs::timeMs(time_ms_t timeMs)
{
    m_timeMs = timeMs;
}

void timeMs::set(time_ms_t timeMs)
{
    m_timeMs = timeMs;
}

void timeMs::setNow()
{
    m_timeMs = nowMs();
}

time_ms_t timeMs::nowMs()
{
    return time_ms();
}

time_ms_t timeMs::get()
{
    return m_timeMs;
}

time_ms_t timeMs::toSeconds()
{
    return m_timeMs / 1000;
}

time_ms_t timeMs::toMinutes()
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

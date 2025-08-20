#ifndef __TIME_MS_HPP__
#define __TIME_MS_HPP__

#include <stdint.h>

#include "api/time/time.hpp"

/**
 * @brief Represents a time in milliseconds.
 * @brief This class is also used to get the current time in milliseconds (from the start of the program).
 */
class TimeMs
{
    private:
    time_ms_t m_timeMs;

    public:
    TimeMs();
    TimeMs(time_ms_t TimeMs);

    /**
     * @brief Set the time.
     *
     * @param TimeMs The time in milliseconds.
     */
    void set(time_ms_t TimeMs);
    /**
     * @brief Set the time to the current time.
     */
    void setNow();

    /**
     * @brief Get the current time.
     *
     * @return time_ms_t The time in milliseconds.
     */
    static time_ms_t nowMs();

    /**
     * @brief Get the time of the object.
     *
     * @return time_ms_t The time in milliseconds.
     */
    time_ms_t get() const;

    /**
     * @brief Convert the time to seconds.
     *
     * @return time_ms_t The time in seconds.
     */
    time_ms_t toSeconds();
    /**
     * @brief Convert the time to minutes.
     *
     * @return time_ms_t The time in minutes.
     */
    time_ms_t toMinutes();

    /**
     * @brief Operators for TimeMs.
     */
    TimeMs operator+(const TimeMs &time);
    TimeMs operator-(const TimeMs &time);
    TimeMs operator*(const TimeMs &time);
    TimeMs operator/(const TimeMs &time);

    bool operator==(const TimeMs &time);
    bool operator!=(const TimeMs &time);
    bool operator>(const TimeMs &time);
    bool operator<(const TimeMs &time);
    bool operator>=(const TimeMs &time);
    bool operator<=(const TimeMs &time);
};

#endif

#ifndef __TIME_MS_H__
#define __TIME_MS_H__

#include <stdint.h>

#include "api/time/time.h"

/**
 * @brief Represents a time in milliseconds.
 * @brief This class is also used to get the current time in milliseconds (from the start of the program).
 */
class timeMs
{
    private:
        time_ms_t m_timeMs;

    public:
        timeMs();
        timeMs(time_ms_t timeMs);

        /**
         * @brief Set the time.
         *
         * @param timeMs The time in milliseconds.
         */
        void set(time_ms_t timeMs);
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
        time_ms_t get();

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
         * @brief Operators for timeMs.
         */
        timeMs operator+(const timeMs &time);
        timeMs operator-(const timeMs &time);
        timeMs operator*(const timeMs &time);
        timeMs operator/(const timeMs &time);

        bool operator==(const timeMs &time);
        bool operator!=(const timeMs &time);
        bool operator>(const timeMs &time);
        bool operator<(const timeMs &time);
        bool operator>=(const timeMs &time);
        bool operator<=(const timeMs &time);
};

#endif

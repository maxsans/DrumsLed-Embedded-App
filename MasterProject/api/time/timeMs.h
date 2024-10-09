#ifndef __TIME_MS_H__
#define __TIME_MS_H__

#include <stdint.h>

/**
 * @brief Represents a time in milliseconds.
 * @brief This class is also used to get the current time in milliseconds (from the start of the program).
 */
class timeMs
{
    private:
        uint64_t m_timeMs;

    public:
        timeMs();
        timeMs(uint64_t timeMs);

        /**
         * @brief Set the time.
         *
         * @param timeMs The time in milliseconds.
         */
        void set(uint64_t timeMs);
        /**
         * @brief Set the time to the current time.
         */
        void setNow();

        /**
         * @brief Get the current time.
         *
         * @return uint64_t The time in milliseconds.
         */
        static uint64_t nowMs();
        /**
         * @brief Get the time of the object.
         *
         * @return uint64_t The time in milliseconds.
         */
        uint64_t get();

        /**
         * @brief Convert the time to seconds.
         *
         * @return uint64_t The time in seconds.
         */
        uint64_t toSeconds();
        /**
         * @brief Convert the time to minutes.
         *
         * @return uint64_t The time in minutes.
         */
        uint64_t toMinutes();

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

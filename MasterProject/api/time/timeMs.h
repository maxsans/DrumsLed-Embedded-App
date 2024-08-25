#ifndef __TIME_MS_H__
#define __TIME_MS_H__

#include <stdint.h>

class timeMs
{
    private:
        uint64_t m_timeMs;

    public:
        timeMs();
        timeMs(uint64_t timeMs);

        void set(uint64_t timeMs);
        void setNow();

        static uint64_t nowMs();
        uint64_t get();

        uint64_t toSeconds();
        uint64_t toMinutes();

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

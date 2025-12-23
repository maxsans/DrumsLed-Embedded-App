#ifndef __CHRONO_MS_HPP__
#define __CHRONO_MS_HPP__

#include "timeMs.hpp"

class ChronoMs
{
    private:
    TimeMs m_start;
    TimeMs m_end;
    bool m_armed;

    public:
    ChronoMs();
    void start();
    void setStart(TimeMs start);
    void arm(TimeMs duration);
    bool ring();
    bool oneTimeRing();
    void restart();

    TimeMs getStart();
    TimeMs getEnd();
    TimeMs getElapsedTime();
};

#endif

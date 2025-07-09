#ifndef __CHRONO_MS_HPP__
#define __CHRONO_MS_HPP__

#include "timeMs

class chronoMs
{
    private:
        timeMs m_start;
        timeMs m_end;
        bool m_armed;

    public:
        chronoMs();
        void start();
        void setStart(timeMs start);
        void arm(timeMs duration);
        bool ring();
        bool oneTimeRing();
        void restart();

        timeMs getStart();
        timeMs getEnd();
        timeMs getElapsedTime();
};

#endif

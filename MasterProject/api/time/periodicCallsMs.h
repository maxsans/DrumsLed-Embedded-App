#ifndef __PERIODIC_CALLS_MS_H__
#define __PERIODIC_CALLS_MS_H__

#include "chronoMs.h"

#include <list>

class periodicCallsMs
{
    private:
        chronoMs m_chrono;
        void (*m_callback)();
        static std::list <periodicCallsMs*> m_instances;

    public:
        periodicCallsMs();
        periodicCallsMs(timeMs period, void (*callback)());
        ~periodicCallsMs();

        static void processAll();
        void process();

        void setPeriod(timeMs period);
        void setCallback(void (*callback)());
};

#endif

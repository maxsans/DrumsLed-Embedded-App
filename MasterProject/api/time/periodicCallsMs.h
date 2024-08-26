#ifndef __PERIODIC_CALLS_MS_H__
#define __PERIODIC_CALLS_MS_H__

#include "chronoMs.h"

#include <list>
#include <stdbool.h>

class periodicCallsMs
{
    private:
        chronoMs m_chrono;
        void (*m_callback)(void*);
        void *m_object;
        bool m_enable;
        static std::list <periodicCallsMs*> m_instances;

    public:
        periodicCallsMs();
        periodicCallsMs(timeMs period, void (*callback)(void* object), void *object);
        ~periodicCallsMs();

        static void processAll();
        void process();

        void setPeriod(timeMs period);
        void setCallback(void (*callback)(void*), void *object);

        void enable(bool enable);
        bool isEnabled();
};

#endif

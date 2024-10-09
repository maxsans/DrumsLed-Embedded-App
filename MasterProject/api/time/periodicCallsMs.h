#ifndef __PERIODIC_CALLS_MS_H__
#define __PERIODIC_CALLS_MS_H__

#include "chronoMs.h"

#include <list>
#include <stdbool.h>

/**
 * @brief Class to call a function periodically.
 */
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

        /**
         * @brief Process all the instances.
         * @note This function must be called in the main loop.
         */
        static void processAll();
        /**
         * @brief Process all the instances.
         * @note This will be automatically called by the processAll function.
         */
        void process();

        /**
         * @brief Set the period.
         *
         * @param period The period in milliseconds.
         */
        void setPeriod(timeMs period);
        /**
         * @brief Set the Callback objectSet the Callback object.
         *
         * @param callback Callback function.
         * @param object Pointer on the object.
         * @note The object can be casted to the desired type in the callback function.
         */
        void setCallback(void (*callback)(void*), void *object);

        /**
         * @brief Enable or disable the periodic call.
         *
         * @param enable True to enable, false to disable.
         */
        void enable(bool enable);
        /**
         * @brief Check if the periodic call is enabled.
         *
         * @return bool True if enabled, false otherwise.
         */
        bool isEnabled();
};

#endif

/**
 * @file periodicCallsMs.hpp
 * @brief Periodic callback system for millisecond-based timed operations
 * @author Cyprien Ménard
 * @date 2025-08-20
 */

#ifndef __PERIODIC_CALLS_MS_HPP__
#define __PERIODIC_CALLS_MS_HPP__

#include "tools/timeTools/chronoMs.hpp"

#include <functional>
#include <stdbool.h>
#include <vector>

/**
 * @brief Periodic callback system for executing functions at regular millisecond intervals
 */
class PeriodicCallsMs
{
    private:
    ChronoMs m_chrono;
    std::function<void()> m_callback;
    bool m_enable;
    static std::vector<PeriodicCallsMs *> m_instances;

    public:
    PeriodicCallsMs();
    PeriodicCallsMs(TimeMs period, std::function<void()> callback);
    ~PeriodicCallsMs();

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
    void setPeriod(TimeMs period);

    /**
     * @brief Set the Callback object.
     *
     * @param callback Callback function.
     * @note Use lambdas or std::bind to capture object context if needed.
     */
    void setCallback(std::function<void()> callback);

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

#endif // __PERIODIC_CALLS_MS_HPP__

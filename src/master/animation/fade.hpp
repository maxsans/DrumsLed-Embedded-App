/**
 * @file fade.hpp
 * @brief Fade animation effect for RGB LEDs triggered by microphone hits
 * @author Cyprien Ménard
 * @date 2025-08-20
 */

#ifndef __FADE_HPP__
#define __FADE_HPP__

#include "animation.hpp"
#include "tools/os/periodicCallsMs/periodicCallsMs.hpp"
#include "tools/timeTools/timeMs.hpp"

#include <stdint.h>

/**
 * @brief Fade animation that triggers on microphone hits and fades LED color over time
 */
class Fade : public Animation
{
    private:
    /**
     * @brief The color to fade to.
     */
    RgbColor m_color;

    /**
     * @brief The duration of the fade in milliseconds.
     */
    timeMs m_duration;

    /**
     * @brief The time when the fade started.
     */
    timeMs m_hitTime;

    /**
     * @brief Periodic call to process the fade animation.
     * @note This is used to call the process() method periodically to update the LED color.
     */
    periodicCallsMs m_periodicCall;

    static void process(void *object);
    void process();

    public:
    Fade(Micro *m, RgbLed *rgbLed, RgbColor color, timeMs duration);
    ~Fade();
    void start();
    void stop();
};

#endif // __FADE_HPP__

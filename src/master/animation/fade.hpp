#ifndef __FADE_HPP__
#define __FADE_HPP__

#include "animation.hpp"
#include "tools/timeTools/periodicCallsMs.hpp"
#include "tools/timeTools/timeMs.hpp"

#include <stdint.h>

/**
 * @brief Fades the LED to a color.
 * @see animation.h for more information.
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

#endif

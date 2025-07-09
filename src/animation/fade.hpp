#ifndef __FADE_HPP__
#define __FADE_HPP__

#include "animation
#include "api/time/time

#include <stdint.h>

/**
 * @brief Fades the LED to a color.
 * @see animation.h for more information.
 */
class Fade : public Animation
{
    private:
        RgbColor m_color;
        uint32_t m_duration;
        time_ms_t m_hitTime;

    public:
        Fade(Micro *m, RgbLed *rgbLed, RgbColor color, uint32_t duration);
        ~Fade();
        void start();
        void process();
        void stop();
};

#endif

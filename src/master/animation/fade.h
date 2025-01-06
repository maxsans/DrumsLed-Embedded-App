#ifndef __FADE_H__
#define __FADE_H__

#include "animation.h"
#include "api/time/time.h"

#include <stdint.h>

/**
 * @brief Fades the LED to a color.
 * @see animation.h for more information.
 */
class fade : public animation
{
    private:
        rgbColor m_color;
        uint32_t m_duration;
        time_ms_t m_hitTime;

    public:
        fade(micro *m, rgbLed *rgbLed, rgbColor color, uint32_t duration);
        ~fade();
        void start();
        void process();
        void stop();
};

#endif

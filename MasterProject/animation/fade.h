#ifndef __FADE_H__
#define __FADE_H__

#include "animation.h"

#include <stdint.h>

class fade : public animation
{
    private:
        rgbColor m_color;
        uint32_t m_duration;
        uint64_t m_hitTime;

    public:
        fade(micro *m, rgbLed *rgbLed, rgbColor color, uint32_t duration);
        ~fade();
        void start();
        void process();
        void stop();
};

#endif

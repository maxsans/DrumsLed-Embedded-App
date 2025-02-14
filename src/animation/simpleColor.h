#ifndef __SIMPLE_COLOR_H__
#define __SIMPLE_COLOR_H__

#include "animation.h"

/**
 * @brief Sets the LED to a color.
 * @see animation.h for more information.
 */
class SimpleColor : public Animation
{
    private:
        RgbColor m_color;

    public:
        SimpleColor(RgbLed *rgbLed, RgbColor color);
        ~SimpleColor();
        void start();
        void process();
        void stop();
};

#endif

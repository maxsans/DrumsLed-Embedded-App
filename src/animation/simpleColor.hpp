#ifndef __SIMPLE_COLOR_HPP__
#define __SIMPLE_COLOR_HPP__

#include "animation.hpp"

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

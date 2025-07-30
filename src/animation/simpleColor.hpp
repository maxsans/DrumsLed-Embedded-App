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
    /**
     * @brief The color to set the LED to.
     */
    RgbColor m_color;

    public:
    SimpleColor(RgbLed *rgbLed, RgbColor color);
    ~SimpleColor();
    void start();
    void stop();
};

#endif

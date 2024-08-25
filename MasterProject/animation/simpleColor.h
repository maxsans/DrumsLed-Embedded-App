#ifndef __SIMPLE_COLOR_H__
#define __SIMPLE_COLOR_H__

#include "animation.h"

class simpleColor : public animation
{
    private:
        rgbColor m_color;

    public:
        simpleColor(rgbLed *rgbLed, rgbColor color);
        ~simpleColor();
        void start();
        void process();
        void stop();
};

#endif

#ifndef __ANIMATION_H__
#define __ANIMATION_H__

#include "micro.h"
#include "rgbLed.h"

typedef enum
{
    ANIMATION_TYPE_NONE,
    ANIMATION_TYPE_SIMPLE_COLOR,
    ANIMATION_TYPE_FADE,
    ANIMATION_TYPE_COUNT
} animationType;

class animation
{
    private:
        animationType m_type;

    protected:
        micro *m_micro;
        rgbLed *m_rgbLed;

    public:
        animation(animationType type, micro *m, rgbLed *rgbLed);
        ~animation();
        animationType getType();
        micro *getMicro();
        rgbLed *getRgbLed();
        virtual void start() = 0;
        virtual void process() = 0;
        virtual void stop() = 0;
};

#endif

#ifndef __COLOR_ORDER_H__
#define __COLOR_ORDER_H__

#include "rgbColor.h"

#include <stdbool.h>

class colorOrder
{
    private:
        rgbColor m_color;
        bool m_enable;

    public:
        colorOrder();
        void setColor(rgbColor color);
        rgbColor getColor();
        bool isEnabled();
        void releaseColor();
};

#endif

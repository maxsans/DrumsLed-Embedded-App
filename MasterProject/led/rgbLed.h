#ifndef __RGB_LED_H__
#define __RGB_LED_H__

#include "rgbColor.h"
#include "module.h"

#include <stdint.h>

class rgbLed
{
    private:
        rgbColor m_color;
        module *m_module;
        uint8_t m_gamma8[256];

    public:
        rgbLed(module *m);
        module *getModule();
        void getColor(uint8_t *redValue, uint8_t *greenValue, uint8_t *blueValue);
        void setColor(uint8_t redValue, uint8_t greenValue, uint8_t blueValue);
        void setColor(uint32_t color);
        uint32_t getColor();
};

#endif

#ifndef __RGB_COLOR_H__
#define __RGB_COLOR_H__

#include <stdint.h>

class rgbColor
{
    private:
        uint8_t m_redValue;
        uint8_t m_greenValue;
        uint8_t m_blueValue;

    protected:

    public:
        rgbColor();
        void setColor(uint8_t redValue, uint8_t greenValue, uint8_t blueValue);
        void setColor(uint32_t color);
        void getColor(uint8_t *redValue, uint8_t *greenValue, uint8_t *blueValue);
        uint32_t getColor();
};

#endif

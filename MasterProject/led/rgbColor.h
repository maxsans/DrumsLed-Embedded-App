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
        rgbColor(uint8_t redValue, uint8_t greenValue, uint8_t blueValue);
        rgbColor(uint32_t color);
        void setColor(uint8_t redValue, uint8_t greenValue, uint8_t blueValue);
        void setColor(uint32_t color);
        void getColor(uint8_t *redValue, uint8_t *greenValue, uint8_t *blueValue);
        uint32_t getColor();

        // Operators
        rgbColor operator*(float value);
};

#endif

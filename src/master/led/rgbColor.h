#ifndef __RGB_COLOR_H__
#define __RGB_COLOR_H__

#include <stdint.h>

/**
 * @brief Class to manage the RGB color.
 */
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
        /**
         * @brief Set the color.
         *
         * @param redValue The red value.
         * @param greenValue The green value.
         * @param blueValue The blue value.
         */
        void setColor(uint8_t redValue, uint8_t greenValue, uint8_t blueValue);
        /**
         * @brief Set the color.
         *
         * @param color The color.
         */
        void setColor(uint32_t color);
        /**
         * @brief Get the color.
         *
         * @param [out] redValue The red value.
         * @param [out] greenValue The green value.
         * @param [out] blueValue The blue value.
         */
        void getColor(uint8_t *redValue, uint8_t *greenValue, uint8_t *blueValue);
        /**
         * @brief Get the color.
         *
         * @return uint32_t The color.
         */
        uint32_t getColor();

        /**
         * @brief Operators for rgbColor.
         */
        rgbColor operator*(float value);
};

#endif

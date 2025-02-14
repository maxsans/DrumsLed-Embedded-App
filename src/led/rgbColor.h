#ifndef __RGB_COLOR_H__
#define __RGB_COLOR_H__

#include <stdint.h>

/**
 * @brief Class to manage the RGB color.
 */
class RgbColor
{
    private:
        uint8_t m_redValue;
        uint8_t m_greenValue;
        uint8_t m_blueValue;

    protected:

    public:
        RgbColor();
        RgbColor(uint8_t redValue, uint8_t greenValue, uint8_t blueValue);
        RgbColor(uint32_t color);
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
         * @brief Get the R/G/B value.
         *
         * @return uint8_t The R/G/B value.
         */
        uint8_t getRed() { return m_redValue; }
        uint8_t getGreen() { return m_greenValue; }
        uint8_t getBlue() { return m_blueValue; }

        /**
         * @brief Operators for rgbColor.
         */
        RgbColor operator*(float value);
};

#endif

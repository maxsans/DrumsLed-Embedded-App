#ifndef __RGB_COLOR_H__
#define __RGB_COLOR_H__

#include <stdint.h>

/**
 * @brief Class to manage the RGB color.
 */
class RgbColor
{
    public:
        typedef union
        {
            uint32_t color;
            struct
            {
                uint8_t b;
                uint8_t g;
                uint8_t r;
            };
        } RawColor;

    private:
        RawColor m_rawColor;

    protected:

    public:
        RgbColor();
        RgbColor(uint8_t redValue, uint8_t greenValue, uint8_t blueValue);
        RgbColor(RawColor raw);
        /**
         * @brief Set the color.
         *
         * @param redValue The red value.
         * @param greenValue The green value.
         * @param blueValue The blue value.
         */
        void setColor(uint8_t redValue, uint8_t greenValue, uint8_t blueValue);
        /**
         * @brief Set the color from raw color value.
         *
         * @param color The color.
         */
        void setColor(RawColor raw);

        /**
         * @brief Get the color.
         *
         * @param [out] redValue The red value.
         * @param [out] greenValue The green value.
         * @param [out] blueValue The blue value.
         */
        void getColor(uint8_t *redValue, uint8_t *greenValue, uint8_t *blueValue);

        /**
         * @brief Get the color as a raw value.
         *
         * @return RawColor The color.
         */
        RawColor getRaw();

        /**
         * @brief Get the R/G/B value.
         *
         * @return uint8_t The R/G/B value.
         */
        uint8_t getRed();
        uint8_t getGreen();
        uint8_t getBlue();

        /**
         * @brief Operators for rgbColor.
         */
        RgbColor operator*(float value);
};

#endif

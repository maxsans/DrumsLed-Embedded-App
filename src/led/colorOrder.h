#ifndef __COLOR_ORDER_H__
#define __COLOR_ORDER_H__

#include "rgbColor.h"

#include <stdbool.h>

/**
 * @brief Class to manage the color order.
 */
class colorOrder
{
    private:
        rgbColor m_color;
        bool m_enable;

    public:
        colorOrder();
        /**
         * @brief Push an order of color.
         * @note If an order is already pushed with higher priority, it will be ignored.
         * @note If the color with higher priority is later released, the color with lower priority will be used.
         *
         * @param color The color to set.
         */
        void setColor(rgbColor color);
        /**
         * @brief Get the Color object
         *
         * @return rgbColor The color.
         */
        rgbColor getColor();
        /**
         * @brief Enable or disable the color.
         *
         * @param enable True to enable, false to disable.
         */
        bool isEnabled();
        /**
         * @brief Release the color.
         * @warning Don't forget to release the color when you don't need it anymore.
         */
        void releaseColor();
};

#endif

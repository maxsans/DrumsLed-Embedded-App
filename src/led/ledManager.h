#ifndef __LED_MANAGER_H__
#define __LED_MANAGER_H__

#include "rgbLed.h"
#include "modules/module.h"
#include "tools/timeTools/periodicCallsMs.h"

#include <vector>

/**
 * @brief Class to manage the LEDs.
 */
class LedManager
{
    private:
        std::vector<RgbLed *> m_leds;
        periodicCallsMs m_updatePeriodicCalls;

    public:
        LedManager();
        ~LedManager();
        /**
         * @brief Add a LED.
         *
         * @param m A pointer on the module to wich the LED is attached.
         */
        void addLed(Module *m);
        /**
         * @brief Get a LED.
         *
         * @param index The index of the LED.
         * @return rgbLed* A pointer on the LED.
         */
        RgbLed *getLed(uint8_t index);
        /**
         * @brief Get a LED.
         *
         * @param m A pointer on the module to wich the LED is attached.
         * @return rgbLed* A pointer on the LED.
         */
        RgbLed *getLed(Module *m);
        /**
         * @brief Get the number of LEDs.
         *
         * @return uint32_t The number of LEDs.
         */
        uint32_t getLedCount();
        /**
         * @brief Internal callback to update the LEDs.
         */
        static void updateCallBack(void *object);
        /**
         * @brief Update the LEDs.
         */
        void update();
        /**
         * @brief Get the module of a LED.
         *
         * @param index The index of the LED.
         * @return module* A pointer on the module.
         */
        Module *getModule(uint8_t index);
        /**
         * @brief Get the module of a LED.
         *
         * @param m A pointer on the module to wich the LED is attached.
         * @return module* A pointer on the module.
         */
        Module *getModule(Module *m);
};

#endif

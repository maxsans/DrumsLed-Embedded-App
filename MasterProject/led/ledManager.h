#ifndef __LED_MANAGER_H__
#define __LED_MANAGER_H__

#include "rgbLed.h"
#include "module.h"
#include "periodicCallsMs.h"

#include <vector>

/**
 * @brief Class to manage the LEDs.
 */
class ledManager
{
    private:
        std::vector<rgbLed *> m_leds;
        periodicCallsMs m_updatePeriodicCalls;

    public:
        ledManager();
        ~ledManager();
        /**
         * @brief Add a LED.
         *
         * @param m A pointer on the module to wich the LED is attached.
         */
        void addLed(module *m);
        /**
         * @brief Get a LED.
         *
         * @param index The index of the LED.
         * @return rgbLed* A pointer on the LED.
         */
        rgbLed *getLed(uint8_t index);
        /**
         * @brief Get a LED.
         *
         * @param m A pointer on the module to wich the LED is attached.
         * @return rgbLed* A pointer on the LED.
         */
        rgbLed *getLed(module *m);
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
        module *getModule(uint8_t index);
        /**
         * @brief Get the module of a LED.
         *
         * @param m A pointer on the module to wich the LED is attached.
         * @return module* A pointer on the module.
         */
        module *getModule(module *m);
};

#endif

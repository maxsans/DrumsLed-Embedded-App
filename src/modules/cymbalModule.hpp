#ifndef __CYMBAL_MODULE_HPP__
#define __CYMBAL_MODULE_HPP__

#include "module.hpp"
#include "micro/micro.hpp"
#include "led/rgbLed.hpp"
#include "tools/timeTools/periodicCallsMs.hpp"

/**
 * @brief Class to represent a cymbal module.
 */
class CymbalModule : public Module
{
    private:
        Micro m_micro;
        RgbLed m_led;
        periodicCallsMs m_updatePeriodicCalls;

    public:
        CymbalModule(Client client);

        /**
         * @brief Overrided process function.
         * @note This function must be called in the main loop.
         */
        void process();

        /**
         * @brief Overrided getMicro function.
         *
         * @return Micro* The micro of the module.
         */
        Micro *getMicro();

        /**
         * @brief Overrided getRgbLed function.
         *
         * @return RgbLed* The rgbLed of the module.
         */
        RgbLed *getRgbLed();

        /**
         * @brief Internal callback to update the LEDs.
         */
        static void updateCallBack(void *object);

        /**
         * @brief Update the LEDs.
         */
        void update();
};

#endif

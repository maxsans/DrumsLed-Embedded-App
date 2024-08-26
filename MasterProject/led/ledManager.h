#ifndef __LED_MANAGER_H__
#define __LED_MANAGER_H__

#include "rgbLed.h"
#include "module.h"
#include "periodicCallsMs.h"

#include <vector>

class ledManager
{
    private:
        std::vector<rgbLed *> m_leds;
        periodicCallsMs m_updatePeriodicCalls;

    public:
        ledManager();
        ~ledManager();
        void addLed(module *m);
        rgbLed *getLed(uint8_t index);
        rgbLed *getLed(module *m);
        uint32_t getLedCount();
        static void updateCallBack(void *object);
        void update();
        module *getModule(uint8_t index);
        module *getModule(module *m);
};

#endif

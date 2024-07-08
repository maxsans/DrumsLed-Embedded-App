#ifndef __LED_MANAGER_H__
#define __LED_MANAGER_H__

#include "rgbLed.h"
#include "module.h"

#include <vector>

class ledManager
{
    private:
        std::vector<rgbLed *> m_leds;

    public:
        ledManager();
        ~ledManager();
        void process();
        void addLed(module *m);
        rgbLed *getLed(uint8_t index);
        rgbLed *getLed(module *m);
        uint32_t getLedCount();
        void update();
        module *getModule(uint8_t index);
        module *getModule(module *m);
};

extern ledManager g_ledManager;

#endif

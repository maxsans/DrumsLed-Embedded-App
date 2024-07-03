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
        void addLed(module *m);
        rgbLed *getLed(uint8_t index);
        rgbLed *getLed(module *m);
        uint32_t getLedCount();
};

extern ledManager g_ledManager;

#endif

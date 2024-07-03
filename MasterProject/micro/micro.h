#ifndef __MICRO_H__
#define __MICRO_H__

#include "module.h"

#include <stdint.h>

class micro
{
    private:

    protected:
        uint8_t m_microValue;
        module *m_module;

    public:
        micro(module *m);
        module *getModule();
        void setMicroValue(uint8_t microValue);
        uint8_t getMicroValue();
};

#endif

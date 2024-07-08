#ifndef __MICRO_H__
#define __MICRO_H__

#include "module.h"
#include "coeff.h"

#include <stdint.h>

class micro
{
    private:

    protected:
        uint8_t m_microValue;
        uint8_t m_microValueCorrected;
        module *m_module;
        coeff m_correction;

    public:
        micro(module *m);
        module *getModule();
        void setCorrection(coeff correction);
        coeff getCorrection();
        void setMicroValue(uint8_t microValue);
        uint8_t getMicroValue();
        uint8_t getMicroValueCorrected();
};

#endif

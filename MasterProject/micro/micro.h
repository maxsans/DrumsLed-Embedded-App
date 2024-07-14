#ifndef __MICRO_H__
#define __MICRO_H__

#include "module.h"
#include "coeff.h"

#include <stdint.h>

class micro
{
    private:

    protected:
        bool m_connected;
        uint8_t m_microValue;
        uint8_t m_microValueCorrected;
        module *m_module;
        coeff m_correction;
        uint8_t m_threshold;
        uint64_t m_lastSyncTime;
        void checkTime();

    public:
        micro(module *m);
        void process();
        void sync();
        module *getModule();
        void setCorrection(coeff correction);
        coeff getCorrection();
        void setMicroValue(uint8_t microValue);
        void setMicroValueCorrected(uint8_t microValueCorrected);
        void setThreshold(uint8_t threshold);
        uint8_t getMicroValue();
        uint8_t getMicroValueCorrected();
        uint8_t getThreshold();
        bool isHit();
};

#endif

#ifndef __MICRO_MANAGER_H__
#define __MICRO_MANAGER_H__

#include "micro.h"
#include "module.h"
#include "impactsManager.h"

#include <vector>

class microManager
{
    private:
        std::vector<micro *> m_micros;
        impactsManager m_impactsManager;

    public:
        microManager();
        ~microManager();
        void process();
        void addMicro(module *m);
        void setMicro(module *m, uint8_t microValue);
        micro *getMicro(int32_t index);
        micro *getMicro(module *m);
        uint32_t getMicroCount();
        impactsManager *getImpactsManager();
};

#endif

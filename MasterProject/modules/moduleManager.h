#ifndef __MODULE_MANAGER_H__
#define __MODULE_MANAGER_H__

#include "module.h"
#include "periodicCallsMs.h"

#include <vector>
#include <stdint.h>
#include <stdbool.h>

class moduleManager
{
    private:
        std::vector<module *> m_modules;
        bool m_enableNewModules;
        periodicCallsMs m_ringPeriodicCalls;
        static void ringCallback(void *object);

    public:
        moduleManager();
        ~moduleManager();
        void enableNewModules(bool enable);
        bool NewModulesEnabled();
        void process();
        bool addModule(char* ip);
        module *getModule(int32_t index);
        module *getModule(char* ip);
        uint32_t getModuleCount();
        void ringModules();
};

#endif

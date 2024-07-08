#ifndef __MODULE_MANAGER_H__
#define __MODULE_MANAGER_H__

#include "module.h"

#include <vector>
#include <stdint.h>
#include <stdbool.h>

class moduleManager
{
    private:
        std::vector<module *> m_modules;

    public:
        moduleManager();
        ~moduleManager();
        void process();
        bool addModule(char* ip);
        module *getModule(int32_t index);
        module *getModule(char* ip);
        int getModuleCount();
        void ringModules();
};

extern moduleManager g_moduleManager;

#endif

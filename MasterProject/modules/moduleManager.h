#ifndef __MODULE_MANAGER_H__
#define __MODULE_MANAGER_H__

#include "module.h"
#include "periodicCallsMs.h"
#include "Client.h"

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
        bool addModule(module *m);
        module *getModule(int32_t index);
        module *getModule(Client client);
        module *getModule(IPv4 ip);
        module *getModule(MacAddr mac);
        uint32_t getModuleCount();
        void ringModules();
};

#endif

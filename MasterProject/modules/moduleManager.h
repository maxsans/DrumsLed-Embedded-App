#ifndef __MODULE_MANAGER_H__
#define __MODULE_MANAGER_H__

#include "module.h"
#include "periodicCallsMs.h"
#include "Client.h"

#include <vector>
#include <stdint.h>
#include <stdbool.h>

/**
 * @brief Class to manage the modules.
 */
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
        /**
         * @brief Enable or disable the addition of new modules.
         *
         * @param enable True to enable, false to disable.
         */
        void enableNewModules(bool enable);
        /**
         * @brief Check if the addition of new modules is enabled.
         *
         * @return True if enabled.
         */
        bool NewModulesEnabled();
        /**
         * @brief Add a module.
         *
         * @param m The module to add.
         * @return True if the module was added.
         */
        bool addModule(module *m);
        /**
         * @brief Get a module.
         *
         * @param index The index of the module.
         * @return module* The module.
         */
        module *getModule(int32_t index);
        /**
         * @brief Get a module.
         *
         * @param client The client of the module.
         * @return module* The module.
         */
        module *getModule(Client client);
        /**
         * @brief Get a module.
         *
         * @param ip The IP of the module.
         * @return module* The module.
         */
        module *getModule(IPv4 ip);
        /**
         * @brief Get a module.
         *
         * @param mac The MAC address of the module.
         * @return module* The module.
         */
        module *getModule(MacAddr mac);
        /**
         * @brief Get the number of modules.
         *
         * @return uint32_t The number of modules.
         */
        uint32_t getModuleCount();
        /**
         * @brief Ping all the modules to check if they are still connected.
         * @note This function must be called periodically.
         */
        void ringModules();
};

#endif

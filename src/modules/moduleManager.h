#ifndef __MODULE_MANAGER_H__
#define __MODULE_MANAGER_H__

#include "modules/module.h"
#include "tools/timeTools/periodicCallsMs.h"
#include "network/client/client.h"
#include "micro/learning/impactsManager.h"

#include <vector>
#include <stdint.h>
#include <stdbool.h>

/**
 * @brief Class to manage the modules.
 */
class ModuleManager
{
    private:
        static std::vector<Module *> m_modules;
        static bool m_enableNewModules;
        static periodicCallsMs m_ringPeriodicCalls;
        static void ringCallback(void *object);
        static ImpactsManager m_impactsManager;

    public:
        ModuleManager() = delete;

        /**
         * @brief Process the modules.
         * @note This function must be called in the main loop.
         */
        static void process();

        /**
         * @brief Enable or disable the addition of new modules.
         *
         * @param enable True to enable, false to disable.
         */
        static void enableNewModules(bool enable);

        /**
         * @brief Check if the addition of new modules is enabled.
         *
         * @return True if enabled.
         */
        static bool NewModulesEnabled();

        /**
         * @brief Add a module.
         *
         * @param type The type of the module.
         * @param client The client of the module.
         *
         * @return module* The module added.
         */
        static Module *addModule(moduleType_t type, Client client);

        /**
         * @brief Get a module.
         *
         * @param index The index of the module.
         * @return module* The module.
         */
        static Module *getModule(int32_t index);

        /**
         * @brief Get a module.
         *
         * @param client The client of the module.
         * @return module* The module.
         */
        static Module *getModule(Client client);

        /**
         * @brief Get a module.
         *
         * @param ip The IP of the module.
         * @return module* The module.
         */
        static Module *getModule(Ipv4 ip);

        /**
         * @brief Get a module.
         *
         * @param mac The MAC address of the module.
         * @return module* The module.
         */
        static Module *getModule(MacAddr mac);

        /**
         * @brief Get the Module of a micro.
         *
         * @param micro The micro.
         * @return module* The module.
         */
        static Module *getModule(Micro *micro);

        /**
         * @brief Get the number of modules.
         *
         * @return uint32_t The number of modules.
         */
        static uint32_t getModuleCount();

        /**
         * @brief Ping all the modules to check if they are still connected.
         * @note This function must be called periodically.
         */
        static void ringModules();

        /**
         * @brief Set the micro value.
         * @note This function will calculate the corrected micro value.
         * @note Then it will call the setMicro function of the module.
         *
         * @param client The client of the module.
         * @param microValue The new micro value.
         */
        static void setMicro(Client client, uint8_t microValue);

        /**
         * @brief Get the impacts manager.
         *
         * @return impactsManager* The impacts manager.
         */
        static ImpactsManager *getImpactsManager();
};

#endif

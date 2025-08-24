#ifndef __MODULE_MANAGER_HPP__
#define __MODULE_MANAGER_HPP__

#include "micro/learning/impactsManager.hpp"
#include "modules/module.hpp"
#include "network/client/client.hpp"
#include "network/interCom/interComParser/interComParser.hpp"
#include "network/interCom/interMsg/interMsgList/interMsgAdc/interMsgAdc.hpp"
#include "network/interCom/interMsg/interMsgList/interMsgInitModule/interMsgInitModule.hpp"
#include "network/interCom/interMsg/interMsgList/interMsgPingSlaves/interMsgPingSlaves.hpp"
#include "tools/os/periodicCallsMs/periodicCallsMs.hpp"

#include <stdbool.h>
#include <stdint.h>
#include <vector>

/**
 * @brief Class to manage all the modules of a session.
 */
class ModuleManager
{
    private:
    /**
     * @brief The interval in milliseconds to ping the modules.
     */
    static const TimeMs m_ringInterval;

    /**
     * @brief The list of modules.
     */
    std::vector<Module *> m_modules;

    /**
     * @brief Flag to set this module manager to be active.
     * @warning Only one module manager should enable this flag at a time.
     * @note Any module manager can be active.
     */
    bool m_active;

    /**
     * @brief The periodic calls to ping the modules.
     * @note This is used to check if the modules are still connected.
     */
    PeriodicCallsMs m_ringPeriodicCalls;

    /**
     * @brief The impacts manager.
     * @note This is used to manage the impacts of module on each other.
     */
    ImpactsManager m_impactsManager;

    /**
     * @brief Set the micro value.
     * @param client The client of the module.
     * @param microValue The new micro value.
     * @note This function will calculate the corrected micro value.
     * @note Then it will call the setMicro function of the module.
     */
    void setMicro(Client client, uint8_t microValue);

    /**
     * @brief Callback to handle a new module detected.
     * @param msg The message received.
     */
    void onNewModule(InterMsgInitModule msg);

    /**
     * @brief Callback to call when a new ADC message is received.
     * @param msg The ADC message.
     */
    void onAdcMsg(InterMsgAdc msg);

    public:
    /**
     * @brief Create an empty ModuleManager.
     */
    ModuleManager(bool active = false);

    /**
     * @see ModuleManager::enable
     * @param e True to enable, false to disable.
     * @warning Only one module manager should enable this flag at a time.
     */
    void enable(bool e);

    /**
     * @brief Check if the addition of new modules is enabled.
     * @return True if enabled.
     */
    bool isActive();

    /**
     * @brief Add a module.
     * @param kitConfig The kit config of the new module.
     * @param client The client of the module.
     * @return module* The module added.
     */
    Module *addModule(KitConfig kitConfig, Client client);

    /**
     * @brief Get a module.
     * @param index The index of the module.
     * @return module* The module.
     */
    Module *getModule(int32_t index);

    /**
     * @brief Get a module.
     * @param client The client of the module.
     * @return module* The module.
     */
    Module *getModule(Client client);

    /**
     * @brief Get a module.
     * @param ip The IP of the module.
     * @return module* The module.
     */
    Module *getModule(Ipv4 ip);

    /**
     * @brief Get a module.
     * @param mac The MAC address of the module.
     * @return module* The module.
     */
    Module *getModule(MacAddr mac);

    /**
     * @brief Get the number of modules.
     * @return uint32_t The number of modules.
     */
    uint32_t getModuleCount();

    /**
     * @brief Ping all the modules to check if they are still connected.
     * @note This function must be called periodically.
     */
    void ringModules();

    /**
     * @brief Get the impacts manager.
     * @return impactsManager* The impacts manager.
     */
    ImpactsManager *getImpactsManager();
};

#endif

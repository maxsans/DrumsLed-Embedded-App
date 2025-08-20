#ifndef __MODULE_HPP__
#define __MODULE_HPP__

#include "kit/kitConfig/kitConfig.hpp"
#include "led/rgbLed.hpp"
#include "micro/micro.hpp"
#include "network/client/client.hpp"
#include "network/interCom/interMsg/interMsg.hpp"
#include "tools/timeTools/periodicCallsMs.hpp"

/**
 * @brief Class to represent a module.
 */
class Module
{
    private:
    /**
     * @brief Timeout for the module to be considered disconnected.
     */
    static const timeMs m_moduleTimeout;

    /**
     * @brief The RGB send interval.
     */
    static const timeMs m_rgbSendInterval;

    /**
     * @brief The kit config of the module.
     */
    KitConfig m_kitConfig;

    /**
     * @brief The client associated with the module.
     */
    Client m_client;

    /**
     * @brief time of the last sync.
     */
    timeMs m_lastSyncTime;

    /**
     * @brief Micro associated with the module.
     */
    Micro *m_micro;

    /**
     * @brief RGB LEDs associated with the module.
     */
    RgbLed *m_rgbLed;

    /**
     * @brief Periodically send Rgb msg to the kits
     */
    periodicCallsMs m_aliveRgbPeriodicCall;
    static void sendRgb(void *object);
    void sendRgb();

    /**
     * @brief Check if the module needs an update.
     * @return true if an update is needed
     * @return false if no update is needed
     */
    bool isUpdateNeeded() const;

    public:
    Module(KitConfig kitConfig, Client client);

    /**
     * @brief Try to update the module.
     * @note This will check if the module needs an update and if so, it will
     * update the module.
     * @return true if the update was successful
     * @return false if the update failed or no update was needed
     */
    bool tryUpdate();

    /**
     * @brief Get the micro of the module.
     * @return Micro* The micro of the module.
     * @return nullptr if no micro is associated with the module.
     */
    Micro *getMicro();

    /**
     * @brief Get the rgbLed of the module.
     * @return RgbLed* The rgbLed of the module.
     * @return nullptr if no rgbLed is associated with the module.
     */
    RgbLed *getRgbLed();

    /**
     * @brief Check if the module is connected.
     * @return true if connected
     * @return false if not connected
     */
    bool isConnected();

    /**
     * @brief Get the config of the module.
     * @return KitConfig The config of the module.
     */
    KitConfig getConfig();

    /**
     * @brief After news over udp, sync the module to push back the timeout.
     */
    void sync();

    /**
     * @brief Get the client of the module.
     * @return Client The client of the module.
     */
    Client getClient();
};

#endif

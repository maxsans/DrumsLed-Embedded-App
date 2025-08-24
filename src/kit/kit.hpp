#ifndef __KIT_HPP__
#define __KIT_HPP__

#include "kit/kitConfig/kitConfig.hpp"
#include "kit/kitService/kitService.hpp"
#include "network/client/client.hpp"
#include "network/interCom/interComParser/interComParser.hpp"
#include "tools/os/periodicCallsMs/periodicCallsMs.hpp"
#include "tools/timeTools/chronoMs.hpp"
#include <vector>

/**
 * @brief How the slave kits see themselves.
 */
class Kit
{
    private:
    /**
     * @brief The client for communication with the slave kit.
     * @note This client is used to communicate with the master kit.
     */
    Client m_masterClient;

    /**
     * @brief The list of services for the slave kit.
     * @note This list contains all the services that the slave kit can provide.
     */
    std::vector<KitService *> m_services;

    /**
     * @brief The timeout for the ping to the master kit.
     * @note This is used to check if the master kit is still connected.
     */
    ChronoMs m_pingTimeout;
    static const TimeMs m_pingTimeoutDuration;

    /**
     * @brief A periodic call to check if the master kit is still connected.
     * @note This is used to ensure that the slave kit can communicate with the master kit.
     */
    PeriodicCallsMs m_timeoutChecker;
    void checkTimeouts();

    /**
     * @brief Check if the master kit is connected and valid.
     * @return True if the master kit is valid, false otherwise.
     * @note This function checks if the master kit is connected and valid.
     *       It is used to ensure that the slave kit can communicate with the master kit.
     */
    bool isMasterValid();

    /**
     * @brief Callback on master kit ping.
     * @param client The client that sent the ping.
     * @param msg The message received.
     * @note This callback is called when a ping message is received from the master kit.
     *       It is used to respond to the ping and update the master client.
     */
    void onPing(const Client &client, InterMsg &msg);

    public:
    /**
     * @brief Create a kit.
     */
    Kit();

    /**
     * @brief Get the kit configuration of the current slave target.
     * @return The kit configuration of the current slave target.
     */
    KitConfig getKitConfig();

    /**
     * @brief Get the master client for communication with the master kit.
     * @return The master client for communication with the master kit.
     */
    Client getMasterClient();
};

#endif // __KIT_HPP__

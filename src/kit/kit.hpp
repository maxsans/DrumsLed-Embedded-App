#ifndef __KIT_HPP__
#define __KIT_HPP__

#include "network/client/client.hpp"
#include "kit/kitConfig/kitConfig.hpp"
#include "network/interCom/interComParser/interComParser.hpp"
#include "kit/kitService/kitService.hpp"
#include <vector>

/**
 * @brief How the slave kits see themselves.
 * @note There is only one way to see oneself, so this class is static.
 */
class Kit
{
    private:
        /**
         * @brief The client for communication with the slave kit.
         * @note This client is used to communicate with the master kit.
         */
        static Client m_masterClient;

        /**
         * @brief The list of services for the slave kit.
         * @note This list contains all the services that the slave kit can provide.
         */
        static std::vector<KitService *> m_services;

        /**
         * @brief Check if the master kit is connected and valid.
         * @return True if the master kit is valid, false otherwise.
         * @note This function checks if the master kit is connected and valid.
         *       It is used to ensure that the slave kit can communicate with the master kit.
         */
        static bool isMasterValid();

        /**
         * @brief Callback on master kit ping.
         * @param client The client that sent the ping.
         * @param msg The message received.
         * @note This callback is called when a ping message is received from the master kit.
         *       It is used to respond to the ping and update the master client.
         */
        static void onPing(const Client &client, InterMsg &msg);

    public:
        /**
         * @brief Initialize the kit slave.
         */
        static void init();

        /**
         * @brief Get the kit configuration of the current slave target.
         * @return The kit configuration of the current slave target.
         */
        static KitConfig getKitConfig();

        /**
         * @brief Get the master client for communication with the master kit.
         * @return The master client for communication with the master kit.
         */
        static Client getMasterClient();
};

#endif // __KIT_HPP__

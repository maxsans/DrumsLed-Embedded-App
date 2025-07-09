#ifndef __KIT_MASTER_HPP__
#define __KIT_MASTER_HPP__

#include "network/client/client
#include "kit/kitConfig/kitConfig.hpp"

/**
 * @brief How the slave kits are seen by the master.
 */
class KitMaster
{
    private:

        /**
         * @brief Config of the slave kit.
         */
        KitConfig m_kitConfig;

        /**
         * @brief Client for communication with the slave kit.
         */
        Client m_client;

    public:
        /**
         * @brief Constructor for KitMaster.
         * @param kitConfig Configuration of the kit.
         * @param client Client for communication with the kit.
         */
        KitMaster(KitConfig kitConfig, Client client);
};

#endif // __KIT_MASTER_HPP__

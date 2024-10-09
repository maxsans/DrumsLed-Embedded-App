#ifndef __MODULE_H__
#define __MODULE_H__

#include "client.h"
#include "periodicCallsMs.h"

/**
 * @brief Class to represent a module.
 */
class module
{
    private:

    protected:
        periodicCallsMs m_checkTimePeriodicCalls;
        Client m_client;
        bool m_connected;
        uint64_t m_lastSyncTime;
        void checkTime();

    public:
        module(Client client);
        /**
         * @brief Check if the module is connected.
         *
         * @return true if connected
         * @return false if not connected
         */
        bool isConnected();
        /**
         * @brief Internal callback.
         */
        static void checkTimeCallBack(void *object);
        /**
         * @brief After news over udp, sync the module to push back the timeout.
         */
        void sync();
        /**
         * @brief Set the IP of the module.
         *
         * @param ip The new sIP of the module
         */
        void setIp(IPv4 ip);
        /**
         * @brief Get the client of the module.
         *
         * @return Client The client of the module.
         */
        Client getClient();
};


#endif

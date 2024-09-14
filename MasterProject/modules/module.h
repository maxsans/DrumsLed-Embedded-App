#ifndef __MODULE_H__
#define __MODULE_H__

#include "client.h"
#include "periodicCallsMs.h"

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
        bool isConnected();
        static void checkTimeCallBack(void *object);
        void sync();
        void setIp(IPv4 ip);
        Client getClient();
};


#endif

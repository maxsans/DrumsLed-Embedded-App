#include "module.h"
#include "timeMs.h"

#define MODULE_TIMEOUT 10000 // ms
#define MODULE_CHECK_TIME_PERIODIC_CALLS 1000 // ms

module::module(Client client)
{
    m_client = client;
    m_connected = false;
    m_lastSyncTime = 0;
    m_checkTimePeriodicCalls = periodicCallsMs(MODULE_CHECK_TIME_PERIODIC_CALLS, checkTimeCallBack, this);
}

bool module::isConnected()
{
    return m_connected;
}

void module::checkTime()
{
    if (timeMs() - m_lastSyncTime > MODULE_TIMEOUT)
    {
        m_connected = false;
    }
}

void module::checkTimeCallBack(void *object)
{
    module *m = (module*)object;
    m->checkTime();
}

void module::sync()
{
    m_lastSyncTime = timeMs::nowMs();
    m_connected = true;
}

void module::setIp(IPv4 ip)
{
    m_client.setIP(ip);
}

Client module::getClient()
{
    return m_client;
}

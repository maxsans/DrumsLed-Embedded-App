#include "module
#include "tools/timeTools/timeMs

#define MODULE_TIMEOUT 10000 // ms
#define MODULE_CHECK_TIME_PERIODIC_CALLS 1000 // ms

Module::Module(moduleType_t moduleType, Client client)
{
    m_moduleType = moduleType;
    m_client = client;
    m_connected = false;
    m_lastSyncTime = 0;
    m_checkTimePeriodicCalls = periodicCallsMs(MODULE_CHECK_TIME_PERIODIC_CALLS, checkTimeCallBack, this);
}

void Module::process()
{
    // Nothing to do here
}

Micro *Module::getMicro()
{
    return nullptr;
}

RgbLed *Module::getRgbLed()
{
    return nullptr;
}

bool Module::isConnected()
{
    return m_connected;
}

moduleType_t Module::getType()
{
    return m_moduleType;
}

void Module::checkTime()
{
    if (timeMs() - m_lastSyncTime > MODULE_TIMEOUT)
    {
        m_connected = false;
    }
}

void Module::checkTimeCallBack(void *object)
{
    Module *m = (Module*)object;
    m->checkTime();
}

void Module::sync()
{
    m_lastSyncTime = timeMs::nowMs();
    m_connected = true;
}

void Module::setIp(Ipv4 ip)
{
    m_client.setIP(ip);
}

Client Module::getClient()
{
    return m_client;
}

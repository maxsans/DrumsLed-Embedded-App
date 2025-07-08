#include "kitSlave.hpp"

#include "kit/kitConfig/kitConfigManager.hpp"
#include "network/interCom/interMsgList/interMsgInitModule/interMsgInitModule.hpp"
#include "tools/logStream/logStream.hpp"

Client KitSlave::m_masterClient;

bool KitSlave::isMasterValid()
{
    // Check if the master has been registered
    return m_masterClient != Client();
}

void KitSlave::onPing(const Client &client, InterMsg &msg)
{
    // Check if the master is already valid
    if (isMasterValid())
    {
        // Check if the client is the same as the master
        if (m_masterClient != client)
        {
            // If the master is already set, force the client to be the master
            // But log a warning
            LogStream::cout << "Warning: Master client already set to " << m_masterClient.toString()
                            << ", but received ping from " << client.toString() << ". Overriding master client." << std::endl;
        }
    }
    // Update the master client
    m_masterClient = client;
    // Respond to the ping
    InterMsgInitModule(client, getKitConfig()).send();
}

void KitSlave::init()
{
    // Register the ping callback
    InterComParser::registerMsgCallback(InterMsgInitModule::m_sendType, onPing);
    // Log the initialization
    LogStream::cout << "KitSlave initialized. Waiting for master client..." << std::endl;
    // Initialize the master client to an invalid state
    m_masterClient = Client();
}

KitConfig KitSlave::getKitConfig()
{
    return KitConfigManager::getKitConfig();
}

Client KitSlave::getMasterClient()
{
    return m_masterClient;
}

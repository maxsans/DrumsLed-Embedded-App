#include "kit.hpp"

#include "kit/kitConfigGenerator/kitConfigGenerator.hpp"
#include "network/interCom/interMsgList/interMsgInitModule/interMsgInitModule.hpp"
#include "tools/logStream/logStream.hpp"

Client Kit::m_masterClient;

// Definition of static member
std::vector<KitService *> Kit::m_services;

bool Kit::isMasterValid()
{
    // Check if the master has been registered
    return m_masterClient != Client();
}

void Kit::onPing(const Client &client, InterMsg &msg)
{
    // Check if the master is already valid
    if (isMasterValid())
    {
        // Check if the client is the same as the master
        if (m_masterClient != client)
        {
            // If the master is already set, force the client to be the master
            // But log a warning
            LogStream::cout << "Warning: Master client already set to " << m_masterClient.getIP().getIpString()
                            << ", but received ping from " << client.getIP().getIpString() << ". Overriding master client." << LogStream::endl;
        }
    }
    // Update the master client
    m_masterClient = client;
    // Respond to the ping
    InterMsgInitModule(m_masterClient, getKitConfig()).send();
}

void Kit::init()
{
    // Register the ping callback
    InterComParser::registerCallback(InterMsgId::InitModule, Kit::onPing);
    // Log the initialization
    LogStream::cout << "Kit initialized. Waiting for master client..." << LogStream::endl;
    // Initialize the master client to an invalid state
    m_masterClient = Client();
    // Generate the kit configuration
    KitConfigGenerator::init();
    // Generate the kit services
    m_services = KitConfigGenerator::generateKitServices();
}

KitConfig Kit::getKitConfig()
{
    return KitConfigGenerator::getKitConfig();
}

Client Kit::getMasterClient()
{
    return m_masterClient;
}

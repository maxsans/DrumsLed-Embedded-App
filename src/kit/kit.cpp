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
    if (!isMasterValid())
    {
        // Update the master client
        m_masterClient = client;
        LogStream::cout << "Master client registered: " << client.getMacAddress() << LogStream::endl;
        // Respond to the ping
        InterMsgInitModule(m_masterClient, getKitConfig()).send();
        LogStream::cout << "Sent InitModule message to master client." << LogStream::endl;
        // Start the services
        for (KitService *service : m_services)
        {
            service->start(m_masterClient);
        }
    }
}

void Kit::init()
{
    // Register the ping callback
    InterComParser::registerCallback(InterMsgId::PingSlaves, Kit::onPing);
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

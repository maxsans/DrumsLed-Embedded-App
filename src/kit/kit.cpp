#include "kit.hpp"

#include "kit/kitConfigGenerator/kitConfigGenerator.hpp"
#include "network/interCom/interMsgList/interMsgInitModule/interMsgInitModule.hpp"
#include "tools/logStream/logStream.hpp"

Client Kit::m_masterClient;
chronoMs Kit::m_pingTimeout;
const timeMs Kit::m_pingTimeoutDuration = 10000;
periodicCallsMs *Kit::m_timeoutChecker = nullptr;

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
        LogStream::cout << "Master client registered: "
                        << client.getIP().getIpString() << LogStream::endl;
        // Respond to the ping
        InterMsgInitModule(m_masterClient, getKitConfig()).send();
        LogStream::cout << "Sent InitModule message to master client."
                        << LogStream::endl;
        // Start the services
        for (KitService *service : m_services)
        {
            service->start(m_masterClient);
        }
    }
    // Restart chrono of ping timeout
    m_pingTimeout.restart();
}

void Kit::init()
{
    // Initialize the timeout checker
    m_timeoutChecker = new periodicCallsMs(
        m_pingTimeoutDuration, &Kit::checkTimeouts, nullptr);
    // Register the ping callback
    InterComParser::registerCallback(InterMsgId::PingSlaves, Kit::onPing);
    // Log the initialization
    LogStream::cout << "Kit initialized. Waiting for master client..."
                    << LogStream::endl;
    // Initialize the master client to an invalid state
    m_masterClient = Client();
    // Generate the kit configuration
    KitConfigGenerator::init();
    // Generate the kit services
    m_services = KitConfigGenerator::generateKitServices();
    // Arm the ping timeout
    m_pingTimeout.arm(m_pingTimeoutDuration);
}

void Kit::checkTimeouts(void *)
{
    // Check if the master client is valid
    if (!isMasterValid())
    {
        return;
    }

    // Check if the ping timeout has occurred
    if (m_pingTimeout.ring())
    {
        LogStream::cout
            << "Ping timeout occurred. Master client is no longer valid."
            << LogStream::endl;
        // Reset the master client
        m_masterClient = Client();
        // Stop all services
        for (KitService *service : m_services)
        {
            service->stop();
        }
    }
}

KitConfig Kit::getKitConfig()
{
    return KitConfigGenerator::getKitConfig();
}

Client Kit::getMasterClient()
{
    return m_masterClient;
}

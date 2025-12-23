#include "kit.hpp"

#include "api/program/program.hpp"
#include "kit/kitConfigGenerator/kitConfigGenerator.hpp"
#include "network/interCom/interMsg/interMsgList/interMsgInitModule/interMsgInitModule.hpp"
#include "network/interCom/interMsg/interMsgList/interMsgOtaSendChunk/interMsgOtaSendChunk.hpp"
#include "network/interCom/interMsg/interMsgList/interMsgOtaStart/interMsgOtaStart.hpp"
#include "network/interCom/interMsg/interMsgList/interMsgPingSlaves/interMsgPingSlaves.hpp"
#include "tools/logStream/logStream.hpp"

const TimeMs Kit::m_pingTimeoutDuration = 10000;

Kit::Kit()
    : m_timeoutChecker(m_pingTimeoutDuration,
                       [this]() { this->checkTimeouts(); })
{
    // Register the ping callback
    InterComParser::registerDeserializer(new InterMsgPingSlaves(
        [this](Client client, InterMsg &msg) { this->onPing(client, msg); }));
    // Register the OTA start callback
    InterComParser::registerDeserializer(
        new InterMsgOtaStart([this](Client client, InterMsg &msg) {
            this->onOtaStart(client, msg);
        }));
    // Register the OTA chunk callback
    InterComParser::registerDeserializer(
        new InterMsgOtaSendChunk([this](Client client, InterMsg &msg) {
            this->onOtaChunk(client, msg);
        }));
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

bool Kit::isMasterValid()
{
    // Check if the master has been registered
    return m_masterClient != Client();
}

void Kit::onPing(Client client, InterMsg &msg)
{
    // Check if the master is already valid
    if (!isMasterValid())
    {
        // Update the master client
        m_masterClient = client;
        LogStream::cout << "Master client registered: "
                        << client.getIP().getIpString() << LogStream::endl;
        LogStream::cout << "Sent InitModule message to master client."
                        << LogStream::endl;
        // Start the services
        for (KitService *service : m_services)
        {
            service->start(m_masterClient);
        }
    }
    // Always respond to the ping to keep the connection alive
    InterMsgInitModule(m_masterClient, getKitConfig()).send();
    // Restart chrono of ping timeout
    m_pingTimeout.restart();
}

void Kit::onOtaStart(Client client, InterMsg &msg)
{
    // Get the OTA start message
    InterMsgOtaStart &otaStartMsg = static_cast<InterMsgOtaStart &>(msg);
    // Get the program length
    int64_t programLength = otaStartMsg.getLength();
    // If the length is valid, start the OTA process
    if (programLength > 0)
    {
        LogStream::cout << "Starting OTA process. Program length: "
                        << programLength << LogStream::endl;
        Program::beginProgramUpdate(programLength);
    }
    else
    {
        LogStream::cout
            << "OTA update not needed. Program is already up to date."
            << LogStream::endl;
        // Validate the last updated program
        Program::validateCurrentFirmware();
    }
}

void Kit::onOtaChunk(Client client, InterMsg &msg)
{
    // Get the OTA chunk message
    InterMsgOtaSendChunk &otaChunkMsg
        = static_cast<InterMsgOtaSendChunk &>(msg);
    // Get the binary data from the message
    Binary binaryData((const uint8_t *)otaChunkMsg.getChunkData(),
                      otaChunkMsg.getChunkSize());
    // Update the program with the received chunk
    if (Program::updateProgramBinary(binaryData))
    {
        LogStream::cout << "Received OTA chunk of size: " << binaryData.size()
                        << LogStream::endl;
        // Check if the chunk is the last one
        if (Program::isTotalSizeReached())
        {
            LogStream::cout << "All OTA chunks received. Finalizing update..."
                            << LogStream::endl;
            // Finalize the OTA update
            if (Program::finalizeProgramUpdate())
            {
                LogStream::cout << "OTA update finalized successfully."
                                << LogStream::endl;
            }
            else
            {
                LogStream::cout << "Failed to finalize OTA update."
                                << LogStream::endl;
            }
        }
    }
    else
    {
        LogStream::cout << "Failed to update program with OTA chunk."
                        << LogStream::endl;
    }
}

void Kit::checkTimeouts()
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

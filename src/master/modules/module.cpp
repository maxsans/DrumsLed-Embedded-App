#include "module.hpp"
#include "api/file/file.hpp"
#include "network/interCom/interComParser/interComParser.hpp"
#include "network/interCom/interMsg/interMsgList/interMsgOtaSendChunk/interMsgOtaSendChunk.hpp"
#include "network/interCom/interMsg/interMsgList/interMsgOtaStart/interMsgOtaStart.hpp"
#include "network/interCom/interMsg/interMsgList/interMsgRgb/interMsgRgb.hpp"
#include "tools/logStream/logStream.hpp"
#include "tools/timeTools/timeMs.hpp"

const TimeMs Module::m_moduleTimeout = TimeMs(5000);
const TimeMs Module::m_rgbSendInterval = TimeMs(20);

Module::Module(KitConfig kitConfig, Client client)
    : m_kitConfig(kitConfig), m_client(client),
      m_aliveRgbPeriodicCall(m_rgbSendInterval, [this]() { this->sendRgb(); }),
      m_micro(nullptr), m_rgbLed(nullptr)
{
    // Add the attributes based on the kit configuration
    if (kitConfig.hasAttributeType(KitAttributeType::Type::LevelAdc))
    {
        m_micro = new Micro();
    }
    if (kitConfig.hasAttributeType(KitAttributeType::Type::Rgb))
    {
        m_rgbLed = new RgbLed();
    }

    // At start, the module is connected
    sync();
}

void Module::sendRgb()
{
    if (m_rgbLed)
    {
        // Send the RGB message to the kits
        InterMsgRgb msgRgb(m_client,
                           m_rgbLed->getColor().getRed(),
                           m_rgbLed->getColor().getGreen(),
                           m_rgbLed->getColor().getBlue());
        msgRgb.send();
    }
}

bool Module::tryUpdate()
{
    // get the binary program from the path available in the kit config
    std::string pathPrefix = "slave-bin/";
    std::string programPath = pathPrefix + m_kitConfig.getProgramPath();
    programPath = File::resolvePath(programPath);
    // Check if the program file exists
    if (!File::exists(programPath))
    {
        LogStream::cout << "Program file does not exist: " << programPath
                        << LogStream::endl;
        return false; // Program file does not exist
    }
    File programFile(programPath);
    programFile.open(programPath, FileMode::READ);

    // Check if the program needs an update
    uint32_t currentCrc32 = programFile.crc32();
    if (currentCrc32 != m_kitConfig.getProgramCrc32())
    {
        LogStream::cout << "Program update needed. Program CRC32: "
                        << m_kitConfig.getProgramCrc32()
                        << ", Current CRC32: " << currentCrc32
                        << " Starting OTA update." << LogStream::endl;
    }
    else
    {
        LogStream::cout << "No program update needed. Program CRC32: "
                        << m_kitConfig.getProgramCrc32()
                        << ", Current CRC32: " << currentCrc32
                        << LogStream::endl;
        return false; // No update needed
    }

    if (!programFile.open(programPath, FileMode::READ))
    {
        LogStream::cout << "Failed to open the program file" << LogStream::endl;
        return false; // Failed to open the program file
    }

    // Send the program chunk by chunk
    const size_t chunkSize = InterMsgOtaSendChunk::m_chunkMaxSize;
    std::vector<char> buffer(chunkSize);
    int64_t totalSize = programFile.size();
    int64_t totalSent = 0;

    // Send the start msg
    InterMsgOtaStart startMsg(m_client, totalSize);
    startMsg.send();

    while (totalSent < totalSize)
    {
        int64_t bytesRead = programFile.read(buffer.data(), chunkSize);
        if (bytesRead <= 0)
        {
            programFile.close();
            LogStream::cout << "Failed to read the program file"
                            << LogStream::endl;
            return false; // Failed to read file
        }

        // Send the chunk
        InterMsgOtaSendChunk chunkMsg(m_client, buffer.data(), bytesRead);
        chunkMsg.send();

        totalSent += bytesRead;
    }

    programFile.close();
    LogStream::cout << "Finished sending OTA update" << LogStream::endl;
    return true;
}

Micro *Module::getMicro()
{
    return m_micro;
}

RgbLed *Module::getRgbLed()
{
    return m_rgbLed;
}

bool Module::isConnected()
{
    return (m_lastSyncTime + m_moduleTimeout) > TimeMs::nowMs();
}

KitConfig Module::getConfig()
{
    return m_kitConfig;
}

void Module::sync()
{
    m_lastSyncTime = TimeMs::nowMs();
}

Client Module::getClient()
{
    return m_client;
}

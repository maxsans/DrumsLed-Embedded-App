#include "kitConfigGenerator.hpp"
#include "api/program/program.hpp"

KitConfig KitConfigGenerator::m_kitConfig;

void KitConfigGenerator::init()
{
    // Call specific initialization based on the kit type
    specificInit();
    // Set the program CRC32 checksum
    // Disabled for now
    ///m_kitConfig.setProgramCrc32(Program::getProgramCrc32());
}

KitConfig KitConfigGenerator::getKitConfig()
{
    return m_kitConfig;
}

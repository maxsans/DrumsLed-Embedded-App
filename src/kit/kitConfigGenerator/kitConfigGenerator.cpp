#include "kitConfigGenerator.hpp"
#include "api/program/program.hpp"

KitConfig KitConfigGenerator::m_kitConfig;

void KitConfigGenerator::init()
{
    // Call specific initialization based on the kit type
    specificInit();
    // Set the program CRC32 checksum from the binary
    Binary programBinary;
    Program::getProgramBinary(&programBinary);
    uint32_t programCrc32 = programBinary.crc32();
    m_kitConfig.setProgramCrc32(programCrc32);
}

KitConfig KitConfigGenerator::getKitConfig()
{
    return m_kitConfig;
}

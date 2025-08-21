#include "kitConfig.hpp"
#include <cstring>

KitConfig::KitConfig(KitType::Type type) : m_type(type)
{
    // Initialize attribute types
    for (int i = 0; i < KitAttributeType::Count; ++i)
    {
        m_attributeTypes[i] = KitAttributeType(KitAttributeType::Type::None, 0);
    }

    // Initialize service types
    for (int i = 0; i < KitServiceType::Count; ++i)
    {
        m_serviceTypes[i] = KitServiceType(KitServiceType::Type::None);
    }

    // Initialize program CRC32
    m_programCrc32 = 0;

    // Initialize program path
    m_programPath[0] = '\0';
}

void KitConfig::setType(KitType::Type type)
{
    m_type = type;
}

void KitConfig::setAttribute(KitAttributeType attributeType,
                             uint32_t nbAttributes)
{
    if (attributeType.getType() < KitAttributeType::Count)
    {
        m_attributeTypes[attributeType.getType()]
            = KitAttributeType(attributeType.getType(), nbAttributes);
    }
}

void KitConfig::setProgramCrc32(uint32_t crc32)
{
    m_programCrc32 = crc32;
}

void KitConfig::setProgramPath(const char *path)
{
    if (path != nullptr)
    {
        strncpy(m_programPath, path, MAX_PROGRAM_PATH_LENGTH - 1);
        m_programPath[MAX_PROGRAM_PATH_LENGTH - 1] = '\0';
    }
    else
    {
        m_programPath[0] = '\0';
    }
}

void KitConfig::setService(KitServiceType serviceType)
{
    if (serviceType.getType() < KitServiceType::Count)
    {
        m_serviceTypes[serviceType.getType()] = serviceType;
    }
}

KitType KitConfig::getType() const
{
    return m_type;
}

bool KitConfig::hasAttributeType(KitAttributeType::Type attributeType) const
{
    if (attributeType < KitAttributeType::Count)
    {
        return m_attributeTypes[attributeType].getType()
               != KitAttributeType::Type::None;
    }
    return false;
}

bool KitConfig::hasServiceType(KitServiceType::Type serviceType) const
{
    if (serviceType < KitServiceType::Count)
    {
        return m_serviceTypes[serviceType].getType()
               != KitServiceType::Type::None;
    }
    return false;
}

uint32_t KitConfig::getProgramCrc32() const
{
    return m_programCrc32;
}

const char *KitConfig::getProgramPath() const
{
    return m_programPath;
}

std::string KitConfig::toString() const
{
    std::string result = "KitConfig: type = " + m_type.toString() + "\n";

    result += "Attributes:\n";
    for (const auto &attributeType : m_attributeTypes)
    {
        if (attributeType.getType() != KitAttributeType::Type::None)
        {
            result += "  - " + attributeType.toString() + "\n";
        }
    }

    result += "Services:\n";
    for (const auto &serviceType : m_serviceTypes)
    {
        if (serviceType.getType() != KitServiceType::Type::None)
        {
            result += "  - " + serviceType.toString() + "\n";
        }
    }

    return result;
}

#include "kitConfig.hpp"
#include "api/network/networkConversion/networkConversion.hpp"
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

KitConfig::KitConfig(const char *rawData)
{
    if (rawData == nullptr)
    {
        // Initialize with default values
        m_type = KitType::Type::None;
        for (int i = 0; i < KitAttributeType::Count; ++i)
        {
            m_attributeTypes[i]
                = KitAttributeType(KitAttributeType::Type::None, 0);
        }
        for (int i = 0; i < KitServiceType::Count; ++i)
        {
            m_serviceTypes[i] = KitServiceType(KitServiceType::Type::None);
        }
        m_programCrc32 = 0;
        m_programPath[0] = '\0';
        return;
    }

    const char *ptr = rawData;

    // Deserialize kit type
    uint32_t typeValue;
    memcpy(&typeValue, ptr, sizeof(uint32_t));
    typeValue = network_ntohl(typeValue);
    m_type = KitType(static_cast<KitType::Type>(typeValue));
    ptr += sizeof(uint32_t);

    // Deserialize attribute types
    for (int i = 0; i < KitAttributeType::Count; ++i)
    {
        uint32_t attrType;
        memcpy(&attrType, ptr, sizeof(uint32_t));
        attrType = network_ntohl(attrType);
        ptr += sizeof(uint32_t);

        uint32_t nbAttributes;
        memcpy(&nbAttributes, ptr, sizeof(uint32_t));
        nbAttributes = network_ntohl(nbAttributes);
        ptr += sizeof(uint32_t);

        m_attributeTypes[i] = KitAttributeType(
            static_cast<KitAttributeType::Type>(attrType), nbAttributes);
    }

    // Deserialize service types
    for (int i = 0; i < KitServiceType::Count; ++i)
    {
        uint32_t serviceType;
        memcpy(&serviceType, ptr, sizeof(uint32_t));
        serviceType = network_ntohl(serviceType);
        ptr += sizeof(uint32_t);

        m_serviceTypes[i]
            = KitServiceType(static_cast<KitServiceType::Type>(serviceType));
    }

    // Deserialize program CRC32
    uint32_t crc32;
    memcpy(&crc32, ptr, sizeof(uint32_t));
    m_programCrc32 = network_ntohl(crc32);
    ptr += sizeof(uint32_t);

    // Deserialize program path
    memcpy(m_programPath, ptr, MAX_PROGRAM_PATH_LENGTH);
    m_programPath[MAX_PROGRAM_PATH_LENGTH - 1]
        = '\0'; // Ensure null termination
}

uint32_t KitConfig::getSerializedSize() const
{
    uint32_t size = 0;

    // Size of kit type
    size += sizeof(uint32_t);

    // Size of attribute types (type + number of attributes for each)
    size += KitAttributeType::Count * (sizeof(uint32_t) + sizeof(uint32_t));

    // Size of service types
    size += KitServiceType::Count * sizeof(uint32_t);

    // Size of program CRC32
    size += sizeof(uint32_t);

    // Size of program path
    size += MAX_PROGRAM_PATH_LENGTH;

    return size;
}

void KitConfig::serialize(char *buffer) const
{
    if (buffer == nullptr)
    {
        return;
    }

    char *ptr = buffer;

    // Serialize kit type
    *reinterpret_cast<uint32_t *>(ptr)
        = network_htonl(static_cast<uint32_t>(m_type.getType()));
    ptr += sizeof(uint32_t);

    // Serialize attribute types
    for (int i = 0; i < KitAttributeType::Count; ++i)
    {
        *reinterpret_cast<uint32_t *>(ptr) = network_htonl(
            static_cast<uint32_t>(m_attributeTypes[i].getType()));
        ptr += sizeof(uint32_t);
        *reinterpret_cast<uint32_t *>(ptr)
            = network_htonl(m_attributeTypes[i].getNbAttributes());
        ptr += sizeof(uint32_t);
    }

    // Serialize service types
    for (int i = 0; i < KitServiceType::Count; ++i)
    {
        *reinterpret_cast<uint32_t *>(ptr)
            = network_htonl(static_cast<uint32_t>(m_serviceTypes[i].getType()));
        ptr += sizeof(uint32_t);
    }

    // Serialize program CRC32
    *reinterpret_cast<uint32_t *>(ptr) = network_htonl(m_programCrc32);
    ptr += sizeof(uint32_t);

    // Serialize program path
    memcpy(ptr, m_programPath, MAX_PROGRAM_PATH_LENGTH);
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

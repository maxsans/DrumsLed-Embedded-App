#include "kitConfig.hpp"

KitConfig::KitConfig(KitType::Type type)
    : m_type(type)
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
}

void KitConfig::setType(KitType::Type type)
{
    m_type = type;
}

void KitConfig::setAttribute(KitAttributeType attributeType, uint32_t nbAttributes)
{
    if (attributeType.getType() < KitAttributeType::Count)
    {
        m_attributeTypes[attributeType.getType()] = KitAttributeType(attributeType.getType(), nbAttributes);
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

KitAttributeType KitConfig::getAttributeType(KitAttributeType::Type attributeType) const
{
    if (attributeType < KitAttributeType::Count)
    {
        return m_attributeTypes[attributeType];
    }
    return KitAttributeType(KitAttributeType::Type::None, 0);
}

KitServiceType KitConfig::getServiceType(KitServiceType::Type serviceType) const
{
    if (serviceType < KitServiceType::Count)
    {
        return m_serviceTypes[serviceType];
    }
    return KitServiceType(KitServiceType::Type::None);
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

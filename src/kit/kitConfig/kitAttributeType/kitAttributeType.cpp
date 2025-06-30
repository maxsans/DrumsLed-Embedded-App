#include "kitAttributeType.hpp"

KitAttributeType::KitAttributeType(Type type, uint32_t nbAttributes)
    : m_type(type), m_nbAttributes(nbAttributes)
{
}

KitAttributeType::Type KitAttributeType::getType() const
{
    return m_type;
}

uint32_t KitAttributeType::getNbAttributes() const
{
    return m_nbAttributes;
}

std::string KitAttributeType::toString() const
{
    return "KitAttributeType: type = " + std::string(m_typeStrings[static_cast<int>(m_type)]) +
           ", nbAttributes = " + std::to_string(m_nbAttributes);
}

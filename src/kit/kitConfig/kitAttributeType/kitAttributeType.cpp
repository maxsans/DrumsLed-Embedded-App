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
    // Check if the type is valid
    if (m_type < Type::Count)
    {
        return "KitAttributeType: type = "
               + std::to_string(static_cast<int>(m_type))
               + ", nbAttributes = " + std::to_string(m_nbAttributes);
    }
    return "KitAttributeType: type = Unknown, nbAttributes = "
           + std::to_string(m_nbAttributes);
}

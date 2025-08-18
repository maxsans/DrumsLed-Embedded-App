#include "kitServiceType.hpp"

KitServiceType::KitServiceType(Type type) : m_type(type)
{
}

KitServiceType::Type KitServiceType::getType() const
{
    return m_type;
}

std::string KitServiceType::toString() const
{
    if (m_type < Type::Count)
    {
        return std::to_string(static_cast<int>(m_type));
    }
    return "Unknown";
}

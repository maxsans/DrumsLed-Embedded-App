#include "kitServiceType.hpp"

KitServiceType::KitServiceType(Type type)
    : m_type(type)
{
}

std::string KitServiceType::toString() const
{
    if (m_type < Type::Count)
    {
        return m_typeStrings[static_cast<size_t>(m_type)];
    }
    return "Unknown";
}

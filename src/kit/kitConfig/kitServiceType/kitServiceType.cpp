#include "kitServiceType.hpp"

// Definition of static constexpr array
constexpr const char* KitServiceType::m_typeStrings[KitServiceType::Count];

KitServiceType::KitServiceType(Type type)
    : m_type(type)
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
        return m_typeStrings[static_cast<size_t>(m_type)];
    }
    return "Unknown";
}

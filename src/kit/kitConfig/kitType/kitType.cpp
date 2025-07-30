#include "kitType.hpp"

// Definition of static constexpr array
constexpr const char *KitType::typeStrings[KitType::Type::Count];

KitType::KitType(Type type) : m_type(type)
{
}

std::string KitType::toString() const
{
    if (m_type < Type::Count)
    {
        return typeStrings[static_cast<size_t>(m_type)];
    }
    return "Unknown";
}

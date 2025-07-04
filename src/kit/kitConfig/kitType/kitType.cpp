#include "kitType.hpp"

KitType::KitType(Type type)
    : m_type(type)
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

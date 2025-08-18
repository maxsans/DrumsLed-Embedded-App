#include "kitType.hpp"

KitType::KitType(Type type) : m_type(type)
{
}

std::string KitType::toString() const
{
    if (m_type < Type::Count)
    {
        return std::to_string(static_cast<int>(m_type));
    }
    return "Unknown";
}

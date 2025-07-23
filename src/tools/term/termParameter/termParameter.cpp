#include "termParameter.hpp"

TermParameter::TermParameter(const std::string& name, const std::string& value)
    : m_name(name), m_value(value)
{
}

const std::string& TermParameter::getName() const
{
    return m_name;
}

const std::string& TermParameter::getValue() const
{
    return m_value;
}

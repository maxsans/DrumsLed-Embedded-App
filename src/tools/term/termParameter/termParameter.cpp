#include "termParameter.hpp"

TermParameter::TermParameter(const std::string &value) : m_value(value)
{
}

const std::string &TermParameter::getValue() const
{
    return m_value;
}

#include "termParameters.hpp"

#include <sstream>

TermParameters::TermParameters(std::string rawParameters)
{
    std::istringstream stream(rawParameters);
    std::string param;

    while (std::getline(stream, param, ';'))
    {
        size_t pos = param.find('=');
        if (pos != std::string::npos)
        {
            std::string name = param.substr(0, pos);
            std::string value = param.substr(pos + 1);
            m_parameters.emplace_back(name, value);
        }
    }
}

std::vector<TermParameter> TermParameters::getParameters() const
{
    return m_parameters;
}

std::string TermParameters::getParameterValue(const std::string& name) const
{
    for (const auto& param : m_parameters)
    {
        if (param.getName() == name)
        {
            return param.getValue();
        }
    }
    return "";
}


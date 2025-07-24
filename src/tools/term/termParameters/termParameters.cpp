#include "termParameters.hpp"

#include <sstream>

TermParameters::TermParameters(std::string rawParameters)
{
    m_parameters.clear();
    std::string param;
    bool inQuotes = false;
    std::string current;
    for (size_t i = 0; i < rawParameters.size(); ++i)
    {
        char c = rawParameters[i];
        if (c == '"')
        {
            inQuotes = !inQuotes;
            continue; // skip the quote
        }
        if (c == ' ' && !inQuotes)
        {
            if (!current.empty())
            {
                m_parameters.emplace_back(current);
                current.clear();
            }
        }
        else
        {
            current += c;
        }
    }
    if (!current.empty())
    {
        m_parameters.emplace_back(current);
    }
}

std::vector<TermParameter> TermParameters::getParameters() const
{
    return m_parameters;
}

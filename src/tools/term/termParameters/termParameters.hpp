#ifndef __TERM_PARAMETERS_HPP__
#define __TERM_PARAMETERS_HPP__

#include "tools/term/termParameter/termParameter.hpp"
#include <string>
#include <vector>

class TermParameters
{
    private:
        std::vector<TermParameter> m_parameters;

    public:
        /**
         * @brief Construct TermParameters from a raw string of parameters.
         * @param rawParameters The raw string containing parameters, e.g., "param1=value1;param2=value2".
         */
        TermParameters(std::string rawParameters);

        /**
         * @brief Get all the parameters.
         * @return A vector of all TermParameter objects.
         */
        std::vector<TermParameter> getParameters() const;

        /**
         * @brief Get the value of a specific parameter by name.
         * @param name The name of the parameter to retrieve.
         * @return The value of the parameter, or an empty string if not found.
         */
        std::string getParameterValue(const std::string& name) const;
};

#endif // __TERM_PARAMETERS_HPP__

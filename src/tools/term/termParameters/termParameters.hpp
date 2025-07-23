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
         * @param rawParameters The raw string containing parameters, e.g., 'param1 param2'
         * @note Array with spaces are supported, e.g., '"array with spaces" anotherParam'
         */
        TermParameters(std::string rawParameters);

        /**
         * @brief Get all the parameters.
         * @return A vector of all TermParameter objects.
         */
        std::vector<TermParameter> getParameters() const;
};

#endif // __TERM_PARAMETERS_HPP__

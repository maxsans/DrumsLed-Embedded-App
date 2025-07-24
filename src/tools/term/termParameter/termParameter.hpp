#ifndef __TERM_PARAMETER_HPP__
#define __TERM_PARAMETER_HPP__

#include <string>

class TermParameter
{
    private:
        std::string m_value;

    public:
        /**
         * @brief Constructor for TermParameter.
         * @param value Initial value of the parameter.
         */
        TermParameter(const std::string& value);

        const std::string& getValue() const;
};

#endif // __TERM_PARAMETER_HPP__

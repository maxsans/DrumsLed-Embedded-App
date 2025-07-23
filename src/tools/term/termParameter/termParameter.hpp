#ifndef __TERM_PARAMETER_HPP__
#define __TERM_PARAMETER_HPP__

#include <string>

class TermParameter
{
    private:
        std::string m_value;
        std::string m_name;

    public:
        /**
         * @brief Constructor for TermParameter.
         * @param value Initial value of the parameter.
         */
        TermParameter(const std::string& name, const std::string& value);

        const std::string& getValue() const;
        const std::string& getName() const;
};

#endif // __TERM_PARAMETER_HPP__

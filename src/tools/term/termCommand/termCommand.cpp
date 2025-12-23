#include "termCommand.hpp"

TermCommand::TermCommand(TermAction action,
                         std::string keyword,
                         std::string description)
    : m_action(action), m_keyword(keyword), m_description(description)
{
}

void TermCommand::execute(TermParameters parameters)
{
    m_action.execute(parameters);
}

std::string TermCommand::getKeyword() const
{
    return m_keyword;
}

std::string TermCommand::getDescription() const
{
    return m_description;
}

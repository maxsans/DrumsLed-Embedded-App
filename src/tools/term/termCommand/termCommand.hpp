#ifndef __TERM_COMMAND_HPP__
#define __TERM_COMMAND_HPP__

#include "tools/term/termAction/termAction.hpp"
#include <string>

class TermCommand
{
    private:
    /**
     * @brief The action to execute when the command is called.
     */
    TermAction m_action;

    /**
     * @brief The keyword that triggers the command.
     */
    std::string m_keyword;

    /**
     * @brief The description of the command.
     */
    std::string m_description;

    public:
    /**
     * @brief Construct a TermCommand with an action, keyword, and description.
     * @param action The action to execute when the command is called.
     * @param keyword The keyword that triggers the command.
     * @param description The description of the command.
     */
    TermCommand(TermAction action,
                std::string keyword,
                std::string description);

    /**
     * @brief Execute the command.
     * @param parameters The parameters to pass to the action.
     */
    void execute(TermParameters parameters);

    /**
     * @brief Get the keyword that triggers the command.
     * @return The keyword that triggers the command.
     */
    std::string getKeyword() const;

    /**
     * @brief Get the description of the command.
     * @return The description of the command.
     */
    std::string getDescription() const;

    /**
     * @brief Equality operator for TermCommand (compares by keyword).
     * @param other The other TermCommand to compare.
     * @return True if keywords are equal, false otherwise.
     */
    bool operator==(const TermCommand &other) const
    {
        return m_keyword == other.m_keyword;
    }
};

#endif // __TERM_COMMAND_HPP__

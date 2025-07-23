#ifndef __TERM_HPP__
#define __TERM_HPP__

#include "termCommand/termCommand.hpp"
#include "api/terminal/terminal.hpp"
#include <vector>
#include <string>

class Term
{
    private:
        /**
         * @brief Native terminal API instance for handling keyboard input.
         */
        static ApiTerminal *m_nativeTerminal;

        /**
         * @brief List of commands available in the terminal.
         */
        static std::vector<TermCommand> m_commands;

        /**
         * @brief Buffer for storing input strings.
         * @note This buffer is reset after each command execution.
         */
        static std::string m_stringBuffer;

        /**
         * @brief Handle a character received from the terminal.
         * @param c The character received.
         */
        static void onCharReceived(char c);

        /**
         * @brief Handle the end of a line received from the terminal.
         * @note This method is called when the user presses Enter.
         * @note This method processes the command entered by the user.
         */
        static void onEndOfLineReceived();

        /**
         * @brief Find a command by its keyword.
         * @param keyword The keyword of the command to find.
         * @return Pointer to the command if found, nullptr otherwise.
         */
        static TermCommand* findCommand(const std::string& keyword);

    public:
        /**
         * @brief Initialize the terminal.
         */
        static void init();

        /**
         * @brief Register a command in the terminal.
         * @param command The command to register.
         */
        static void registerCommand(const TermCommand& command);

        /**
         * @brief Unregister a command from the terminal.
         * @param keyword The keyword of the command to unregister.
         */
        static void unregisterCommand(const std::string& keyword);
};

#endif // __TERM_HPP__

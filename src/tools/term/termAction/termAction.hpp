/**
 * @file termAction.hpp
 * @brief Terminal command action handler
 * @author Cyprien Ménard
 * @date 2025-08-20
 */

#ifndef __TERM_ACTION_HPP__
#define __TERM_ACTION_HPP__

#include "tools/term/termParameters/termParameters.hpp"
#include <functional>

/**
 * @brief Handler for terminal command actions with callback functionality
 */
class TermAction
{
    public:
    /**
     * @brief Callback type to call on command call.
     * @param parameters The parameters of the command.
     */
    using CommandCallback
        = std::function<void(const TermParameters &parameters)>;

    private:
    /**
     * @brief The callback to call when the action is triggered.
     */
    CommandCallback m_callback;

    public:
    /**
     * @brief Constructor for TermAction.
     * @param callback The callback to call when the action is triggered.
     */
    TermAction(CommandCallback callback);

    /**
     * @brief Execute the action with the given parameters.
     * @param parameters The parameters to pass to the callback.
     */
    void execute(const TermParameters &parameters) const;
};

#endif // __TERM_ACTION_HPP__

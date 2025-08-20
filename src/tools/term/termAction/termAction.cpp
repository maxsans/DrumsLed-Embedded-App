/**
 * @file termAction.cpp
 * @author Cyprien Ménard
 * @date 2025-08-20
 */

#include "termAction.hpp"
#include "tools/async/async.hpp"

TermAction::TermAction(CommandCallback callback) : m_callback(callback)
{
}

void TermAction::execute(const TermParameters &parameters) const
{
    if (m_callback)
    {
        m_callback(parameters);
    }
}

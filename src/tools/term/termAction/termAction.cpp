#include "termAction.hpp"

TermAction::TermAction(CommandCallback callback)
    : m_callback(callback)
{
}

void TermAction::execute(const TermParameters& parameters) const
{
    if (m_callback)
    {
        m_callback(parameters);
    }
}

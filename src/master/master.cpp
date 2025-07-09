#include "master.hpp"

Session Master::m_currentSession;

void Master::init()
{
    // Initialize the current session.
    m_currentSession = new Session();
}

Session& Master::getCurrentSession()
{
    return *m_currentSession;
}

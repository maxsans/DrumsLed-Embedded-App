#include "session.h"

session::session()
{
    m_learning.setSession(&m_moduleManager, &m_microManager, &m_ledManager);
}

session::~session()
{
}

void session::process()
{
    m_moduleManager.process();
    m_ledManager.process();
    m_learning.process();
}

microManager *session::getMicroManager()
{
    return &m_microManager;
}

ledManager *session::getLedManager()
{
    return &m_ledManager;
}

moduleManager *session::getModuleManager()
{
    return &m_moduleManager;
}

learning *session::getLearning()
{
    return &m_learning;
}

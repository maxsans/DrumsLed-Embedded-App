#include "session.h"

Session::Session() : m_learning(&m_moduleManager)
{
}

Session::~Session()
{
}

void Session::process()
{
    m_moduleManager.process();
    m_animationManager.process();
}

ModuleManager *Session::getModuleManager()
{
    return &m_moduleManager;
}

learning *Session::getLearning()
{
    return &m_learning;
}

AnimationManager *Session::getAnimationManager()
{
    return &m_animationManager;
}

Session g_session;
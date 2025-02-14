#include "session.h"

Session::Session()
{
    m_learning.setSession(&m_moduleManager, &m_microManager, &m_ledManager);
}

Session::~Session()
{
}

void Session::process()
{
    m_microManager.process();
    m_animationManager.process();
}

MicroManager *Session::getMicroManager()
{
    return &m_microManager;
}

LedManager *Session::getLedManager()
{
    return &m_ledManager;
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
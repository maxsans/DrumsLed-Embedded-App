#include "session.hpp"

Session::Session()
{
}

Session::~Session()
{
}

void Session::process()
{
    m_animationManager.process();
}

AnimationManager *Session::getAnimationManager()
{
    return &m_animationManager;
}

#include "session.hpp"

Session::Session()
{
}

Session::~Session()
{
}

AnimationManager *Session::getAnimationManager()
{
    return &m_animationManager;
}

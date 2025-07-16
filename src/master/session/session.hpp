#ifndef __SESSION_HPP__
#define __SESSION_HPP__

#include "animation/animationManager.hpp"

class Session
{
    private:
        AnimationManager m_animationManager;

    public:
        Session();
        ~Session();
        AnimationManager *getAnimationManager();
};

#endif

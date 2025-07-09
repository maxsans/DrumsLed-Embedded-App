#ifndef __SESSION_HPP__
#define __SESSION_HPP__

#include "animation/animationManager

class Session
{
    private:
        AnimationManager m_animationManager;

    public:
        Session();
        ~Session();
        void process();
        AnimationManager *getAnimationManager();
};

#endif

#ifndef __SESSION_H__
#define __SESSION_H__

#include "modules/moduleManager.h"
#include "micro/learning/learning.h"
#include "animation/animationManager.h"

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

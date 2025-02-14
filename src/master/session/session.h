#ifndef __SESSION_H__
#define __SESSION_H__

#include "modules/moduleManager.h"
#include "micro/learning/learning.h"
#include "animation/animationManager.h"

class Session
{
    private:
        ModuleManager m_moduleManager;
        learning m_learning;
        AnimationManager m_animationManager;

    public:
        Session();
        ~Session();
        void process();
        ModuleManager *getModuleManager();
        learning *getLearning();
        AnimationManager *getAnimationManager();
};

extern Session g_session;

#endif

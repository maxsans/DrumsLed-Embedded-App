#ifndef __SESSION_H__
#define __SESSION_H__

#include "micro/microManager.h"
#include "led/ledManager.h"
#include "modules/moduleManager.h"
#include "micro/learning/learning.h"
#include "animation/animationManager.h"

class Session
{
    private:
        MicroManager m_microManager;
        LedManager m_ledManager;
        ModuleManager m_moduleManager;
        learning m_learning;
        AnimationManager m_animationManager;

    public:
        Session();
        ~Session();
        void process();
        MicroManager *getMicroManager();
        LedManager *getLedManager();
        ModuleManager *getModuleManager();
        learning *getLearning();
        AnimationManager *getAnimationManager();
};

extern Session g_session;

#endif

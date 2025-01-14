#ifndef __SESSION_H__
#define __SESSION_H__

#include "micro/microManager.h"
#include "led/ledManager.h"
#include "modules/moduleManager.h"
#include "micro/learning/learning.h"
#include "animation/animationManager.h"

class session
{
    private:
        microManager m_microManager;
        ledManager m_ledManager;
        moduleManager m_moduleManager;
        learning m_learning;
        animationManager m_animationManager;

    public:
        session();
        ~session();
        void process();
        microManager *getMicroManager();
        ledManager *getLedManager();
        moduleManager *getModuleManager();
        learning *getLearning();
        animationManager *getAnimationManager();
};

extern session g_session;

#endif

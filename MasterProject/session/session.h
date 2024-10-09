#ifndef __SESSION_H__
#define __SESSION_H__

#include "microManager.h"
#include "ledManager.h"
#include "moduleManager.h"
#include "learning.h"
#include "animationManager.h"

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

#endif

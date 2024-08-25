#ifndef __ANIMATION_MANAGER_H__
#define __ANIMATION_MANAGER_H__

#include "animation.h"

#include <vector>

class animationManager
{
    private:
        std::vector<animation*> m_animations;

    public:
        animationManager();
        ~animationManager();
        void process();
        void addAnimation(animation* anim);
        void removeAnimation(uint32_t index);
        void removeAnimation(animation* anim);
};

#endif

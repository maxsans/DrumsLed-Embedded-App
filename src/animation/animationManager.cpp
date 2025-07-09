#include "animationManager.h"

#include <assert.h>

AnimationManager::AnimationManager()
{
}

AnimationManager::~AnimationManager()
{
    for (std::vector<Animation*>::iterator it = m_animations.begin(); it != m_animations.end(); ++it)
    {
        delete *it;
    }
}

void AnimationManager::process()
{
    for (std::vector<Animation*>::iterator it = m_animations.begin(); it != m_animations.end(); ++it)
    {
        (*it)->process();
    }
}

void AnimationManager::addAnimation(Animation* anim)
{
    m_animations.push_back(anim);
    anim->start();
}

void AnimationManager::removeAnimation(uint32_t index)
{
    assert(index < m_animations.size());
    m_animations[index]->stop();
    delete m_animations[index];
    m_animations.erase(m_animations.begin() + index);
}

void AnimationManager::removeAnimation(Animation* anim)
{
    for(uint32_t l_index = 0; l_index < m_animations.size(); ++l_index)
    {
        if(m_animations[l_index] == anim)
        {
            removeAnimation(l_index);
        }
    }
}

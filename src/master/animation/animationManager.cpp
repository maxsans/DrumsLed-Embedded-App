#include "animationManager.h"

#include <assert.h>

animationManager::animationManager()
{
}

animationManager::~animationManager()
{
    for (std::vector<animation*>::iterator it = m_animations.begin(); it != m_animations.end(); ++it)
    {
        delete *it;
    }
}

void animationManager::process()
{
    for (std::vector<animation*>::iterator it = m_animations.begin(); it != m_animations.end(); ++it)
    {
        (*it)->process();
    }
}

void animationManager::addAnimation(animation* anim)
{
    m_animations.push_back(anim);
    anim->start();
}

void animationManager::removeAnimation(uint32_t index)
{
    assert(index < m_animations.size());
    m_animations[index]->stop();
    delete m_animations[index];
    m_animations.erase(m_animations.begin() + index);
}

void animationManager::removeAnimation(animation* anim)
{
    for(uint32_t l_index = 0; l_index < m_animations.size(); ++l_index)
    {
        if(m_animations[l_index] == anim)
        {
            removeAnimation(l_index);
        }
    }
}

#ifndef __ANIMATION_MANAGER_H__
#define __ANIMATION_MANAGER_H__

#include "animation.h"

#include <vector>

/**
 * @brief Manages animations.
 */
class animationManager
{
    private:
        std::vector<animation*> m_animations;

    public:
        animationManager();
        ~animationManager();
        /**
         * @brief Process all animations.
         * @note This function must be called in the main loop.
         */
        void process();
        /**
         * @brief Add an animation.
         *
         * @param anim A pointer to the animation object to add.
         */
        void addAnimation(animation* anim);
        /**
         * @brief Remove an animation.
         *
         * @param index The index of the animation to remove.
         */
        void removeAnimation(uint32_t index);
        /**
         * @brief Remove an animation.
         *
         * @param anim A pointer to the animation object to remove.
         */
        void removeAnimation(animation* anim);
};

#endif

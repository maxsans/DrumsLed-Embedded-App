#ifndef __ANIMATION_MANAGER_HPP__
#define __ANIMATION_MANAGER_HPP__

#include "animation.hpp"

#include <vector>

/**
 * @brief Manages animations.
 */
class AnimationManager
{
    private:
        std::vector<Animation*> m_animations;

    public:
        AnimationManager();
        ~AnimationManager();
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
        void addAnimation(Animation* anim);
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
        void removeAnimation(Animation* anim);
};

#endif

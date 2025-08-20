/**
 * @file animationManager.hpp
 * @brief Manager for LED animations lifecycle and execution
 * @author Cyprien Ménard
 * @date 2025-08-20
 */

#ifndef __ANIMATION_MANAGER_HPP__
#define __ANIMATION_MANAGER_HPP__

#include "animation.hpp"

#include <vector>

/**
 * @brief Manages LED animation objects including creation, execution, and cleanup
 */
class AnimationManager
{
    private:
    std::vector<Animation *> m_animations;

    public:
    AnimationManager();
    ~AnimationManager();

    /**
     * @brief Add an animation.
     *
     * @param anim A pointer to the animation object to add.
     */
    void addAnimation(Animation *anim);

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
    void removeAnimation(Animation *anim);
};

#endif // __ANIMATION_MANAGER_HPP__

#ifndef __ANIMATION_HPP__
#define __ANIMATION_HPP__

#include "micro/micro.hpp"
#include "led/rgbLed.hpp"

/**
 * @brief Enumerates the animation types.
 */
typedef enum
{
    ANIMATION_TYPE_NONE,
    ANIMATION_TYPE_SIMPLE_COLOR,
    ANIMATION_TYPE_FADE,
    ANIMATION_TYPE_COUNT
} animationType_t;

/**
 * @brief Base class for animations.
 */
class Animation
{
    private:
        /**
        * @brief The type of the animation.
        */
        animationType_t m_type;

    protected:
        /**
         * @brief Pointer to the Micro object.
         */
        Micro *m_micro;

        /**
         * @brief Pointer to the RgbLed object.
         */
        RgbLed *m_rgbLed;

    public:
        Animation(animationType_t type, Micro *m, RgbLed *rgbLed);
        ~Animation();

        /**
         * @brief Get the type of the animation.
         *
         * @return animationType
         */
        animationType_t getType();

        /**
         * @brief Get the micro object.
         *
         * @return micro* A pointer to the micro object.
         */
        Micro *getMicro();

        /**
         * @brief Get the rgbLed object.
         *
         * @return rgbLed* A pointer to the rgbLed object.
         */
        RgbLed *getRgbLed();

        /**
         * @brief Start the animation.
         * @note This function is pure virtual and must be implemented by the derived class.
         */
        virtual void start() = 0;

        /**
         * @brief Stop the animation.
         * @note This function is pure virtual and must be implemented by the derived class.
         */
        virtual void stop() = 0;
};

#endif

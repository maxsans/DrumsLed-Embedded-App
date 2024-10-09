#ifndef __ANIMATION_H__
#define __ANIMATION_H__

#include "micro.h"
#include "rgbLed.h"

/**
 * @brief Enumerates the animation types.
 */
typedef enum
{
    ANIMATION_TYPE_NONE,
    ANIMATION_TYPE_SIMPLE_COLOR,
    ANIMATION_TYPE_FADE,
    ANIMATION_TYPE_COUNT
} animationType;

/**
 * @brief Base class for animations.
 */
class animation
{
    private:
        animationType m_type;

    protected:
        micro *m_micro;
        rgbLed *m_rgbLed;

    public:
        animation(animationType type, micro *m, rgbLed *rgbLed);
        ~animation();
        /**
         * @brief Get the type of the animation.
         *
         * @return animationType
         */
        animationType getType();
        /**
         * @brief Get the micro object.
         *
         * @return micro* A pointer to the micro object.
         */
        micro *getMicro();
        /**
         * @brief Get the rgbLed object.
         *
         * @return rgbLed* A pointer to the rgbLed object.
         */
        rgbLed *getRgbLed();
        /**
         * @brief Start the animation.
         * @note This function is pure virtual and must be implemented by the derived class.
         */
        virtual void start() = 0;
        /**
         * @brief Process the animation.
         * @note This function is pure virtual and must be implemented by the derived class.
         */
        virtual void process() = 0;
        /**
         * @brief Stop the animation.
         * @note This function is pure virtual and must be implemented by the derived class.
         */
        virtual void stop() = 0;
};

#endif

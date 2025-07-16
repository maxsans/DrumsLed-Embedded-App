#ifndef __SESSION_HPP__
#define __SESSION_HPP__

#include "animation/animationManager.hpp"
#include "modules/moduleManager.hpp"
#include "micro/learning/learning.hpp"

class Session
{
    private:
        /**
         * @brief Indicates if the session is active.
         * @note This is used to determine if the session is running or not.
         * @warning Only one session can be active at a time.
         */
        bool m_active;

        /**
         * @brief Animation manager to handle animations.
         */
        AnimationManager m_animationManager;

        /**
         * @brief Module manager to handle modules.
         */
        ModuleManager m_moduleManager;

        /**
         * @brief Learning manager to handle learning sessions.
         * @note If no learning session is active, it will be nullptr.
         * @note If a learning session is active, it will point to the Learning object.
         */
        Learning *m_learning;

        /**
         * @brief Callback to call when the learning is done.
         * @note This callback is called when the learning session is finished.
         */
        static void learningDoneCallback(void *object);
        void processLearningDone();

    public:
        Session(bool active = false);
        ~Session();

        /**
         * @brief Set the session as active.
         */
        void setActive(bool active);

        /**
         * @brief Check if the session is active.
         * @return true if the session is active, false otherwise.
         */
        bool isActive() const;

        /**
         * @brief Start a learning session on every module.
         */
        void startLearning();

        /**
         * @brief Stop the current learning session.
         */
        void stopLearning();
};

#endif

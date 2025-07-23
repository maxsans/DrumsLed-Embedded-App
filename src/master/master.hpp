#ifndef __KIT_MASTER_HPP__
#define __KIT_MASTER_HPP__

#include "session/session.hpp"

/**
 * @brief How the master see itself.
 * @note There is only one way to see oneself, so this class is static.
 */
class Master
{
    private:
        // The master session.
        static Session *m_currentSession;

    public:
        /**
         * @brief Initialize the master.
         * @note This function should be called once at the beginning of the program.
         */
        static void init();

        /**
         * @brief Get the current session.
         * @return The current session.
         */
        static Session& getCurrentSession();
};

#endif // __KIT_MASTER_HPP__

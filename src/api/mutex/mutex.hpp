#ifndef __MUTEX_HPP__
#define __MUTEX_HPP__

/**
 * @brief Portable mutex implementation that adapts to the current
 * platform
 */
class Mutex
{
    private:
    /**
     * @brief Private data for platform-specific mutex implementation
     */
    void *m_privateData = nullptr;

    public:
    /**
     * @brief Constructor - initializes the mutex for the current
     */
    Mutex();

    /**
     * @brief Destructor - cleans up -specific resources
     */
    ~Mutex();

    // Disable copy constructor and assignment operator for thread safety
    Mutex(const Mutex &) = delete;
    Mutex &operator=(const Mutex &) = delete;

    /**
     * @brief Lock the mutex
     */
    void lock();

    /**
     * @brief Unlock the mutex
     */
    void unlock();

    /**
     * @brief RAII lock guard for automatic lock/unlock
     */
    class LockGuard
    {
        private:
        Mutex &m_mutex;

        public:
        LockGuard(Mutex &mutex) : m_mutex(mutex)
        {
            m_mutex.lock();
        }
        ~LockGuard()
        {
            m_mutex.unlock();
        }
    };
};

#endif // __MUTEX_HPP__

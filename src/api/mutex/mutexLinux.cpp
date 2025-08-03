#include "mutex.hpp"
#include <pthread.h>

Mutex::Mutex()
{
    pthread_mutex_t* mutex = new pthread_mutex_t;
    pthread_mutex_init(mutex, nullptr);
    m_privateData = mutex;
}

Mutex::~Mutex()
{
    if (m_privateData)
    {
        pthread_mutex_t* mutex = static_cast<pthread_mutex_t*>(m_privateData);
        pthread_mutex_destroy(mutex);
        delete mutex;
    }
}

void Mutex::lock()
{
    pthread_mutex_lock(static_cast<pthread_mutex_t*>(m_privateData));
}

void Mutex::unlock()
{
    pthread_mutex_unlock(static_cast<pthread_mutex_t*>(m_privateData));
}

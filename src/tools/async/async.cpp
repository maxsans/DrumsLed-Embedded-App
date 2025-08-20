/**
 * @file async.cpp
 * @author Cyprien Ménard
 * @date 2025-08-20
 */

#include "async.hpp"
#include <algorithm>

std::vector<Async *> Async::m_asyncOperations;
Mutex Async::m_mutex;

void Async::process()
{
    std::vector<Async *> toProcess;
    {
        Mutex::LockGuard lock(m_mutex);
        toProcess.swap(
            m_asyncOperations); // Take ownership and clear the shared list
    }

    for (auto it : toProcess)
    {
        {
            Mutex::LockGuard instanceLock(it->m_instanceMutex);
            if (it->m_callback && !it->m_processed)
            {
                it->m_callback();
                it->m_processed = true;
            }
        }

        if (it->m_mustDelete)
        {
            delete it;
        }
    }
}

Async::Async(AsyncCallback callback, bool mustDelete)
    : m_callback(callback), m_mustDelete(mustDelete), m_processed(false)
{
    Mutex::LockGuard lock(m_mutex);
    m_asyncOperations.push_back(this);
}

Async::Async(AsyncCallback callback) : Async(callback, false)
{
}

bool Async::isDone() const
{
    Mutex::LockGuard instanceLock(m_instanceMutex);
    return m_processed;
}

void Async::registerAsync(AsyncCallback callback)
{
    // Create a new Async instance and do not keep a reference to it
    // Mark the instance for deletion
    new Async(callback, true);
}

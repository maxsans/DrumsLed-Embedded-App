#include "async.hpp"
#include <algorithm>

std::vector<Async *> Async::m_asyncOperations;
volatile bool Async::m_locked = false;

void Async::lock()
{
    while (Async::m_locked) { /* busy wait */ }
    Async::m_locked = true;
}

void Async::unlock()
{
    Async::m_locked = false;
}

void Async::process()
{
    std::vector<Async *> toProcess;
    {
        lock();
        toProcess.swap(m_asyncOperations); // Take ownership and clear the shared list
        unlock();
    }
    for (auto it : toProcess)
    {
        if (it->m_callback)
        {
            it->m_callback();
        }
        if (it->m_mustDelete)
        {
            delete it;
        }
    }
    // No need to clear m_asyncOperations here, it's already swapped out
}

Async::Async(AsyncCallback callback, bool mustDelete)
    : m_callback(callback),
      m_mustDelete(mustDelete)
{
    lock();
    m_asyncOperations.push_back(this);
    unlock();
}

Async::Async(AsyncCallback callback)
    : Async(callback, false) // Default to not delete after processing
{
}

void Async::registerAsync(AsyncCallback callback)
{
    // Create a new Async instance and do not keep a reference to it
    // Mark the instance for deletion
    new Async(callback, true);
}

bool Async::isDone() const
{
    lock();
    bool done = std::find(m_asyncOperations.begin(), m_asyncOperations.end(), this) == m_asyncOperations.end();
    unlock();
    return done;
}

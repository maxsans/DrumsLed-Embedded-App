#include "async.hpp"
#include <algorithm>
#include <mutex>

std::vector<Async *> Async::m_asyncOperations;
std::mutex Async::m_mutex;

void Async::process()
{
    std::vector<Async *> toProcess;
    {
        std::lock_guard<std::mutex> lock(m_mutex);
        toProcess.swap(m_asyncOperations); // Take ownership and clear the shared list
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
    std::lock_guard<std::mutex> lock(m_mutex);
    m_asyncOperations.push_back(this);
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
    std::lock_guard<std::mutex> lock(m_mutex);
    return std::find(m_asyncOperations.begin(), m_asyncOperations.end(), this) == m_asyncOperations.end();
}

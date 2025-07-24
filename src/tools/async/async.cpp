#include "async.hpp"

std::vector<Async *> Async::m_asyncOperations;
periodicCallsMs Async::m_processHandler(0, Async::processAsync, nullptr);

void Async::processAsync(void *object)
{
    // Process all registered async operations
    for (auto it : m_asyncOperations)
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
    // Clear the list of async operations after processing
    m_asyncOperations.clear();
}

Async::Async(AsyncCallback callback, bool mustDelete)
    : m_callback(callback),
      m_mustDelete(mustDelete)
{
    // Register this async operation
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
    // Find if this async operation is in the list
    return std::find(m_asyncOperations.begin(), m_asyncOperations.end(), this) == m_asyncOperations.end();
}

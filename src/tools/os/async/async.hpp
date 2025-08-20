/**
 * @file async.hpp
 * @brief Asynchronous operation handler for main loop execution
 * @author Cyprien Ménard
 * @date 2025-08-20
 */

#ifndef __ASYNC_HPP__
#define __ASYNC_HPP__

#include "api/mutex/mutex.hpp"
#include "tools/os/periodicCallsMs/periodicCallsMs.hpp"
#include <functional>
#include <vector>

/**
 * @brief Class to handle asynchronous operations that will be called on next main loop iteration
 */
class Async
{
    public:
    /**
     * @brief Callback to call asynchronously
     */
    using AsyncCallback = std::function<void()>;

    private:
    /**
     * @brief List of async operations to call
     */
    static std::vector<Async *> m_asyncOperations;

    /**
     * @brief Mutex to protect the async operations list
     */
    static Mutex m_mutex;

    /**
     * @brief Callback to call when the async operation is done
     */
    AsyncCallback m_callback;

    /**
     * @brief Flag to indicate if the async operation must be deleted after being processed
     */
    bool m_mustDelete;

    /**
     * @brief Flag to indicate if the async operation has been processed
     */
    mutable bool m_processed;

    /**
     * @brief Mutex to protect the instance state
     */
    mutable Mutex m_instanceMutex;

    /**
     * @brief Private constructor to register the async operation
     * @param callback The callback to call when the async operation is done
     * @param mustDelete Flag to indicate if the async operation must be deleted after being processed
     */
    Async(AsyncCallback callback, bool mustDelete);

    public:
    /**
     * @brief Start the async operation.
     * @param callback The callback to call when the async operation is done.
     */
    Async(AsyncCallback callback);

    /**
     * @brief Process all registered async operations.
     * @note This function may be called in the main loop.
     */
    static void process();

    /**
     * @brief Register an async operation without keeping a reference to it.
     * @param callback The async operation to do.
     */
    static void registerAsync(AsyncCallback callback);

    /**
     * @brief Check if the async operation has been done.
     * @return true if the async operation has been done, false otherwise.
     */
    bool isDone() const;
};

#endif // __ASYNC_HPP__

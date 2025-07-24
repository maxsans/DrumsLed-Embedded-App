#ifndef __ASYNC_HPP__
#define __ASYNC_HPP__

#include <functional>
#include <vector>
#include "tools/timeTools/periodicCallsMs.hpp"

/**
 * @brief Class to handle asynchronous operations
 * @details The registered operations will be called on next main loop iteration.
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
         * @brief Periodic process handler for the async operation
         */
        static periodicCallsMs m_processHandler;
        static void processAsync(void *object);

        /**
         * @brief Callback to call when the async operation is done
         */
        AsyncCallback m_callback;

        /**
         * @brief Flag to indicate if the async operation must be deleted after being processed
         */
        bool m_mustDelete;

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

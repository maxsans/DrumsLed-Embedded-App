/**
 * @file terminal.hpp
 * @brief Cross-platform terminal API for character input and screen control
 * @author Cyprien Ménard
 * @date 2025-08-20
 */

#ifndef __TERMINAL_HPP__
#define __TERMINAL_HPP__

#include <atomic>
#include <thread>

/**
 * @brief Cross-platform terminal interface with threaded character input handling
 */
class ApiTerminal
{
    public:
    /**
     * @brief Callback when a character is received.
     * @param c The character received.
     */
    typedef void (*onCharReceivedCallback)(char c);

    private:
    /**
     * @brief Callback to call when a character is received.
     */
    onCharReceivedCallback m_onCharReceivedCallback;

    /**
     * @brief Flag to indicate if the terminal is running.
     */
    std::atomic<bool> m_running;

    /**
     * @brief Thread that checks for keyboard input.
     */
    std::thread m_thread;
    void threadFunc();

    public:
    /**
     * @brief Constructor for ApiTerminal.
     * @param callback Callback to call when a character is received.
     */
    ApiTerminal(onCharReceivedCallback callback);
    ~ApiTerminal();

    /**
     * @brief Clear the output of the terminal.
     * @note This method clears the terminal screen.
     */
    void clear();

    friend void pollTerminalInput(ApiTerminal *terminal);
};

#endif // __TERMINAL_HPP__

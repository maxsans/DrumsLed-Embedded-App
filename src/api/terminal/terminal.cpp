#include "terminal.hpp"
#include <chrono>

// Les fonctions suivantes doivent être définies dans chaque fichier plateforme :
extern bool terminal_kbhit();
extern char terminal_getch();
extern void terminal_platform_init();
extern void terminal_platform_cleanup();

ApiTerminal::ApiTerminal(onCharReceivedCallback callback)
    : m_onCharReceivedCallback(callback), m_running(true)
{
    terminal_platform_init();
    m_thread = std::thread(&ApiTerminal::threadFunc, this);
}

ApiTerminal::~ApiTerminal()
{
    m_running = false;
    if (m_thread.joinable())
        m_thread.join();
    terminal_platform_cleanup();
}

void ApiTerminal::threadFunc()
{
    while (m_running)
    {
        if (terminal_kbhit())
        {
            char c = terminal_getch();
            if (m_onCharReceivedCallback)
            {
                m_onCharReceivedCallback(c);
            }
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }
}

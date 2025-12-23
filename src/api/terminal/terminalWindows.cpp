/**
 * @file terminalWindows.cpp
 * @author Cyprien Ménard
 * @date 2025-08-20
 */

#include "terminal.hpp"
#include <chrono>
#include <conio.h>
#include <iostream>

// Pas d'init ni cleanup spécifique pour Windows
void terminal_platform_init()
{
}
void terminal_platform_cleanup()
{
}

bool terminal_kbhit()
{
    return _kbhit() != 0;
}

char terminal_getch()
{
    return _getch();
}

void ApiTerminal::clear()
{
    // ANSI escape code to clear the screen and move cursor to home
    std::cout << "\033[2J\033[H" << std::endl;
}

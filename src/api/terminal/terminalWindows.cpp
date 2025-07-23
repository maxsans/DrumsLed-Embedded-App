#include "terminal.hpp"
#include <conio.h>
#include <chrono>

// Pas d'init ni cleanup spécifique pour Windows
void terminal_platform_init() {}
void terminal_platform_cleanup() {}

bool terminal_kbhit() {
    return _kbhit() != 0;
}

char terminal_getch() {
    return _getch();
}

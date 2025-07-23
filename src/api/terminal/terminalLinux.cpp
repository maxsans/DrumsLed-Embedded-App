#include "terminal.hpp"
#include <termios.h>
#include <unistd.h>
#include <fcntl.h>
#include <chrono>
#include <iostream>

static struct termios orig_termios;

void terminal_platform_init()
{
    struct termios new_termios;
    tcgetattr(STDIN_FILENO, &orig_termios);
    new_termios = orig_termios;
    new_termios.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &new_termios);
}

void terminal_platform_cleanup()
{
    tcsetattr(STDIN_FILENO, TCSANOW, &orig_termios);
}

bool terminal_kbhit()
{
    int old_flags = fcntl(STDIN_FILENO, F_GETFL);
    fcntl(STDIN_FILENO, F_SETFL, old_flags | O_NONBLOCK);
    int c = getchar();
    fcntl(STDIN_FILENO, F_SETFL, old_flags);
    if (c != EOF) {
        ungetc(c, stdin);
        return true;
    }
    return false;
}

char terminal_getch()
{
    return getchar();
}

void ApiTerminal::clear()
{
    // ANSI escape code to clear the screen and move cursor to home
    std::cout << "\033[2J\033[H" << std::endl;
}

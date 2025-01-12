#include "logs.h"

#include <stdio.h>
#include <stdarg.h>

void logs_init()
{
    // Nothing to do here
}

void log(const char *format, ...)
{
    if (ENABLE_LOGS)
    {
        va_list args;
        va_start(args, format);
        vprintf(format, args);
        va_end(args);
    }
}

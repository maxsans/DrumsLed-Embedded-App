#include "time.hpp"

#include <windows.h>

void time_init()
{
    // Initialize time
}

time_us_t time_us()
{
    LARGE_INTEGER frequency, counter;
    QueryPerformanceFrequency(&frequency);
    QueryPerformanceCounter(&counter);
    return (time_us_t)((counter.QuadPart * 1000000) / frequency.QuadPart);
}

time_ms_t time_ms()
{
    return GetTickCount();
}

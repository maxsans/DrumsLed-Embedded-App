#include "time.h"

#include <sys/time.h>
#include <unistd.h>

void time_init()
{
    // Initialize time
}

time_us_t time_us()
{
    struct timeval tv;
    gettimeofday(&tv, NULL);
    return (time_us_t)(tv.tv_sec * 1000000 + tv.tv_usec);
}

time_ms_t time_ms()
{
    struct timeval tv;
    gettimeofday(&tv, NULL);
    return (time_ms_t)(tv.tv_sec * 1000 + tv.tv_usec / 1000);
}

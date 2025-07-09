#include "time

#include <sys/time.h>
#include <esp_timer.h>

void time_init()
{
    // Initialize time
}

time_us_t time_us()
{
    return esp_timer_get_time();
}

time_ms_t time_ms()
{
    return esp_timer_get_time() / 1000;
}

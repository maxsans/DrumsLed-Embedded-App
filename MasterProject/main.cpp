
#include "udp.h"
#include "moduleManager.h"

#include <sys/time.h>
#include <stdio.h>

int main()
{
    g_udp.init();

    while(1)
    {
        g_udp.recv();
        // Send a broadcast UDP packet to ring new modules every seconds
        static struct timeval l_lastTrigger;
        struct timeval l_currentTime;
        gettimeofday(&l_currentTime, NULL);
        if (l_currentTime.tv_sec - l_lastTrigger.tv_sec > 1)
        {
            l_lastTrigger = l_currentTime;
            g_moduleManager.ringModules();
        }
    }
    g_udp.close();
    return 0;
}

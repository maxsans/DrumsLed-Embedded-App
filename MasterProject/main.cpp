
#include "udp.h"
#include "moduleManager.h"
#include "ledManager.h"
#include "microManager.h"

#include <sys/time.h>
#include <stdio.h>

int main()
{
    g_udp.init();

    while(1)
    {
        g_udp.recv();
        g_moduleManager.process();
        g_ledManager.process();
    }
    g_udp.close();
    return 0;
}

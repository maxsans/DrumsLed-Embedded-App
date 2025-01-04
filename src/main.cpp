
#ifdef __TARGET_MASTER
    #include "masterLaunch.h"
#elif defined __TARGET_SLAVES
    #include "slavesLaunch.h"
#endif

#ifdef ESP_PLATFORM
    extern "C" void app_main()
#else
    int main()
#endif
    {
        #ifdef __TARGET_MASTER
            masterLaunch();
        #elif defined __TARGET_SLAVES
            slavesLaunch();
        #endif
    }

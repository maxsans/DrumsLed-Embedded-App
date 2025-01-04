#include "launch.h"

#ifdef ESP_PLATFORM
    extern "C" void app_main()
#else
    int main()
#endif
    {
        launch();
    }

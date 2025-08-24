include(cmake/tools.cmake)
include(cmake/targets/slave/setup_sdkconfig.cmake)

# Setup sdkconfig for ESP8266 slave builds
setup_sdkconfig_for_esp8266()

# Preprocessor definitions
add_compile_options(-D__TARGET_SLAVES)

####################################################################################################
# Slaves source files (from src directory)
####################################################################################################
add_sources(
    slaves/slavesLaunch.cpp
    api/target/esp8266.cpp
    api/mutex/mutexEsp8266.cpp
    api/logs/logsEsp8266.cpp
    api/udp/udpEsp8266.cpp
    api/tcp/tcpEsp8266.cpp
    api/time/timeEsp8266.cpp
    api/wifi/wifiEsp8266.cpp
    api/drivers/ws2812Esp8266/ws2812Esp8266.cpp
    kit/kitConfigGenerator/kitConfigGenerator.cpp
    kit/kitService/srvcLevelAdcSend/srvcLevelAdcSend.cpp
    kit/kitService/srvcRgbListen/srvcRgbListen.cpp
    kit/kitService/kitService.cpp
    kit/kit.cpp
)

####################################################################################################
# Slaves include directories (from src directory)
####################################################################################################
add_include_dirs(
    slaves
)

# Add the specific cmake file for the target
include(${CMAKE_CURRENT_LIST_DIR}/slave_custom/${CUSTOM_CMAKE_NAME}.cmake)

# Print the list of sources and include directories
print_app_info()

# Add the specific cmake file for esp8266
include(cmake/targets/slave/esp8266.cmake)

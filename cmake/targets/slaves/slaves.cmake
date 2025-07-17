
include(cmake/tools.cmake)

# Preprocessor definitions
add_compile_options(-D__TARGET_SLAVES)

####################################################################################################
# Slaves source files (from src directory)
####################################################################################################
add_sources(
    slaves/slavesLaunch.cpp
    slaves/kit/kit.cpp
    kit/kit.hpp
    api/target/esp8266.cpp
    api/logs/logsEsp8266.cpp
    api/udp/udpEsp8266.cpp
    api/tcp/tcpEsp8266.cpp
    api/time/timeEsp8266.cpp
    api/wifi/wifiEsp8266.cpp
    kit/kitConfig/kitConfigManager/kitConfigManager.cpp
    )

####################################################################################################
# Slaves include directories (from src directory)
####################################################################################################
add_include_dirs(
    slaves
    )

# Add the specific cmake file for the target
if(${TARGET} STREQUAL "DrumKit")
    include(cmake/targets/slaves/drumkit.cmake)
elseif(${TARGET} STREQUAL "CymbalKit")
    include(cmake/targets/slaves/cymbalkit.cmake)
endif()

# Print the list of sources and include directories
print_sources_and_include_dirs()

# Add the specific cmake file for esp8266
include(cmake/targets/slaves/esp8266.cmake)

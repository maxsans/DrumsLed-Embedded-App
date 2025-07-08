
include(cmake/tools.cmake)

# Preprocessor definitions
add_compile_options(-D__TARGET_MASTER)

####################################################################################################
# Master source files (from src directory)
####################################################################################################
add_sources(
    master/masterLaunch.cpp
    master/session/session.cpp

    kit/kitMaster/kitMaster.cpp

    animation/animation.cpp
    animation/animationManager.cpp
    animation/fade.cpp
    animation/simpleColor.cpp

    led/colorOrder.cpp
    led/rgbLed.cpp

    micro/learning/impact.cpp
    micro/learning/impactsManager.cpp
    micro/learning/learning.cpp
    micro/learning/record.cpp
    micro/learning/recordSlot.cpp
    micro/micro.cpp

    modules/module.cpp
    modules/drumModule.cpp
    modules/cymbalModule.cpp
    modules/moduleManager.cpp
    )

# Specific windows source files
if(WIN32)
    add_sources(
        api/logs/logsWindows.cpp
        api/udp/udpWindows.cpp
        api/time/timeWindows.cpp
        api/tcp/tcpWindows.cpp
        )
endif()

# Specific linux source files
if(UNIX)
    add_sources(
        api/logs/logsLinux.cpp
        api/udp/udpLinux.cpp
        api/time/timeLinux.cpp
        )
endif()

####################################################################################################
# Master include directories (from src directory)
####################################################################################################
add_include_dirs(
    master
    )

# Print the sources and include directories
print_sources_and_include_dirs()

add_executable(${TARGET} ${SRCS})
target_include_directories(${TARGET} PRIVATE ${INCLUDE_DIRS})

####################################################################################################
# Link libraries
####################################################################################################

# Common libraries

# Windows libraries
if(WIN32)
    target_link_libraries(${TARGET} Ws2_32)
    target_link_libraries(${TARGET} iphlpapi)
endif()

# Linux libraries
if(UNIX)
    target_link_libraries(${TARGET} pthread)
endif()

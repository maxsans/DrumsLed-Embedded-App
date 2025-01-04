
include(cmake/tools.cmake)

# Preprocessor definitions
add_compile_options(-D__TARGET_MASTER)

####################################################################################################
# Master source files (from src directory)
####################################################################################################
add_sources(
    master/masterLaunch.cpp

    master/animation/animation.cpp
    master/animation/animationManager.cpp
    master/animation/fade.cpp
    master/animation/simpleColor.cpp

    master/modules/module.cpp
    master/modules/moduleManager.cpp

    master/led/colorOrder.cpp
    master/led/ledManager.cpp
    master/led/rgbColor.cpp
    master/led/rgbLed.cpp

    master/micro/learning/impact.cpp
    master/micro/learning/impactsManager.cpp
    master/micro/learning/learning.cpp
    master/micro/learning/record.cpp
    master/micro/learning/recordSlot.cpp
    master/micro/micro.cpp
    master/micro/microManager.cpp
    )

# Specific windows source files
if(WIN32)
    add_sources(
        api/udp/udpWindows.cpp
        api/time/timeWindows.cpp
        )
endif()

# Specific linux source files
if(UNIX)
    add_sources(
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
endif()

# Linux libraries
if(UNIX)
    target_link_libraries(${TARGET} pthread)
endif()

include(cmake/tools.cmake)

# Preprocessor definitions
add_compile_options(-D__TARGET_MASTER)

####################################################################################################
# Master source files (from src directory)
####################################################################################################
add_sources(
    master/masterLaunch.cpp
    master/session/session.cpp

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
    modules/moduleManager.cpp

    tools/term/term.cpp
    tools/term/termCommand/termCommand.cpp
    tools/term/termAction/termAction.cpp
    tools/term/termParameters/termParameters.cpp
    tools/term/termParameter/termParameter.cpp

    api/terminal/terminal.cpp
)

# Add sources from platform-specific files
include(${CMAKE_CURRENT_LIST_DIR}/master_custom/${CUSTOM_CMAKE_NAME}.cmake)

####################################################################################################
# Master include directories (from src directory)
####################################################################################################
add_include_dirs(
    master
)

# Print the sources and include directories
print_app_info()

# Add .exe to the target name
set(TARGET ${TARGET}.exe)

# Create the executable target
add_executable(${TARGET} ${SRCS})
target_include_directories(${TARGET} PRIVATE ${INCLUDE_DIRS})

####################################################################################################
# Link libraries
####################################################################################################

target_link_libraries(${TARGET} ${LIBS})

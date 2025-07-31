
# Make sure the IDF_PATH environment variable is set
if(NOT DEFINED ENV{IDF_PATH})
    # Use the default path if the IDF_PATH environment variable is not set
    set(ENV{IDF_PATH} "$ENV{HOME}/esp/ESP8266_RTOS_SDK")
    message(WARNING "IDF_PATH environment variable is not set. Using default path: $ENV{IDF_PATH}")
endif()

# Make sure .../xtensa-lx106-elf/bin is in the PATH
if(NOT DEFINED ENV{PATH})
    set(ENV{PATH} "$ENV{IDF_PATH}/xtensa-lx106-elf/bin:$ENV{PATH}")
else()
    string(FIND "$ENV{PATH}" "xtensa-lx106-elf/bin" PATH_INDEX)
    if(PATH_INDEX EQUAL -1)
        # Add the default path to the PATH environment variable
        set(ENV{PATH} "$ENV{HOME}/esp/xtensa-lx106-elf/bin:$ENV{PATH}")
        message(WARNING "xtensa-lx106-elf/bin is not in the PATH. Adding default path: $ENV{IDF_PATH}/xtensa-lx106-elf/bin")
    endif()
endif()

if(POLICY CMP0077)
    cmake_policy(SET CMP0077 NEW)
endif()

if(POLICY CMP0115)
    cmake_policy(SET CMP0115 NEW)
endif()

# Avoid some warnings
set(CMAKE_SUPPRESS_DEVELOPER_WARNINGS 1 CACHE BOOL "No dev warnings")
set(CMAKE_SUPPRESS_DEPRECATED_WARNINGS 1 CACHE BOOL "No deprecated warnings")
if(NOT CMAKE_CXX_FLAGS MATCHES "-Wno-deprecated")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-deprecated")
endif()

# Include the project.cmake file from the IDF_PATH
include($ENV{IDF_PATH}/tools/cmake/project.cmake)

# Enable back the Warnings
set(CMAKE_SUPPRESS_DEVELOPER_WARNINGS 0 CACHE BOOL "Enable dev warnings")
set(CMAKE_SUPPRESS_DEPRECATED_WARNINGS 0 CACHE BOOL "Enable deprecated warnings")

# Main component directory
set(EXTRA_COMPONENT_DIRS "./src")

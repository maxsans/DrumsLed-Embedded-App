cmake_minimum_required(VERSION 3.5)

include(cmake/tools.cmake)

# List of possible targets
set(POSSIBLE_TARGETS
    "Master"
    "DrumKit"
    "CymbalKit"
    )
# List of targets that are slaves
set(SLAVES_TARGETS "DrumKit" "CymbalKit")
# Make sure TARGET is set and valid
if(NOT DEFINED TARGET)
    message(FATAL_ERROR "TARGET is not set.\n"
                        "Please pass a valid target to the build system using -DTARGET=<target>\n"
                        "Possible targets are: ${POSSIBLE_TARGETS}")
elseif(NOT ${TARGET} IN_LIST POSSIBLE_TARGETS)
    message(FATAL_ERROR "TARGET is invalid.\n"
                        "Please pass a valid target to the build system using -DTARGET=<target>\n"
                        "Possible targets are: ${POSSIBLE_TARGETS}")
endif()

# Set the SRC_PREFIX
if(${TARGET} STREQUAL "Master")
    set(SRC_PREFIX src)
elseif(${TARGET} IN_LIST SLAVES_TARGETS)
    set(SRC_PREFIX ".")
endif()

####################################################################################################
# Common source files for all targets (from src directory)
####################################################################################################
add_sources(
    main.cpp

    api/logs/logStream.cpp

    tools/maths/matrix/matrix.cpp
    tools/timeTools/timeMs.cpp
    tools/timeTools/chronoMs.cpp
    tools/timeTools/periodicCallsMs.cpp

    network/client/client.cpp
    network/client/ipv4.cpp
    network/client/macAddr.cpp
    network/udpParser/udpParser.cpp
    network/udpParser/udpPackets/udpPacket.cpp
    network/udpParser/udpPackets/udpPacketAdc.cpp
    network/udpParser/udpPackets/udpPacketInit.cpp
    network/udpParser/udpPackets/udpPacketRgb.cpp
    )

####################################################################################################
# Common include directories for all targets (from src directory)
####################################################################################################
add_include_dirs(
    .
    )

# Import the specific cmake file for each target
if(${TARGET} STREQUAL "Master")
    include(cmake/targets/master/master.cmake)
elseif(${TARGET} IN_LIST SLAVES_TARGETS)
    include(cmake/targets/slaves/slaves.cmake)
else()
    message(FATAL_ERROR "TARGET is invalid.\n"
                        "Please pass a valid target to the build system using -DTARGET=<target>\n"
                        "Possible targets are: ${POSSIBLE_TARGETS}")
endif()

project(${TARGET} CXX)

cmake_minimum_required(VERSION 3.5)

# Args :
#   TARGET_CMAKE_NAME : Name of the target cmake file (e.g., "slave", "master")
#   CUSTOM_CMAKE_NAME : Custom name for the target (e.g., "drumkit", "cymbalkit", linux, windows)
message(STATUS "Target CMake Name: ${TARGET_CMAKE_NAME}")
message(STATUS "Custom CMake Name: ${CUSTOM_CMAKE_NAME}")

set(PROJECT_NAME ${TARGET_CMAKE_NAME}-${CUSTOM_CMAKE_NAME})
project(${PROJECT_NAME} CXX)

# Set SRC_DIR to the source directory
set(SRC_DIR "${CMAKE_CURRENT_SOURCE_DIR}/src")

# Set TARGET to the project name for use in add_executable
set(TARGET ${PROJECT_NAME})

include(cmake/tools.cmake)

####################################################################################################
# Common source files for all targets (from src directory)
####################################################################################################
add_sources(
    main.cpp

    kit/kitConfig/kitAttributeType/kitAttributeType.cpp
    kit/kitConfig/kitServiceType/kitServiceType.cpp
    kit/kitConfig/kitType/kitType.cpp
    kit/kitConfig/kitConfig.cpp

    tools/maths/matrix/matrix.cpp
    tools/timeTools/timeMs.cpp
    tools/timeTools/chronoMs.cpp
    tools/timeTools/periodicCallsMs.cpp
    tools/logStream/logStream.cpp
    tools/async/async.cpp

    network/client/client.cpp
    network/client/ipv4.cpp
    network/client/macAddr.cpp

    network/interCom/interComParser/interComParser.cpp
    network/interCom/interMsg/interMsg.cpp
    network/interCom/interMsgData/interMsgHeader/interMsgHeader.cpp
    network/interCom/interMsgData/interMsgData.cpp
    network/interCom/interMsgId/interMsgId.cpp
    network/interCom/interMsgList/interMsgGeneric/interMsgGeneric.cpp
    network/interCom/interMsgList/interMsgExample/interMsgExample.cpp
    network/interCom/interMsgList/interMsgAdc/interMsgAdc.cpp
    network/interCom/interMsgList/interMsgInitModule/interMsgInitModule.cpp
    network/interCom/interMsgList/interMsgPingSlaves/interMsgPingSlaves.cpp
    network/interCom/interMsgList/interMsgRgb/interMsgRgb.cpp
    network/interCom/interMsgList/interMsgAlive/interMsgAlive.cpp

    led/rgbColor.cpp
)

####################################################################################################
# Common include directories for all targets (from src directory)
####################################################################################################
add_include_dirs(
    .
)

# Import the specific cmake file
include(cmake/targets/${TARGET_CMAKE_NAME}/${TARGET_CMAKE_NAME}.cmake)

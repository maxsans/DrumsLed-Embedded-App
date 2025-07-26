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
    # Automatically copy sdkconfig.default to sdkconfig if sdkconfig does not exist
    if(NOT EXISTS "${CMAKE_SOURCE_DIR}/sdkconfig")
        file(COPY "${CMAKE_SOURCE_DIR}/sdkconfig.default" DESTINATION "${CMAKE_BINARY_DIR}")
        file(RENAME "${CMAKE_BINARY_DIR}/sdkconfig.default" "${CMAKE_BINARY_DIR}/sdkconfig")
        file(COPY "${CMAKE_BINARY_DIR}/sdkconfig" DESTINATION "${CMAKE_SOURCE_DIR}")
        file(REMOVE "${CMAKE_BINARY_DIR}/sdkconfig.default")
    endif()
endif()

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
project(${TARGET} CXX)

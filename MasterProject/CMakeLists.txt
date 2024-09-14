cmake_minimum_required(VERSION 3.5.0)
project(MasterProject VERSION 0.1.0 LANGUAGES C CXX)

set(SRCS_FILES
    main.cpp

    animation/animation.cpp
    animation/animationManager.cpp
    animation/fade.cpp
    animation/simpleColor.cpp

    api/terminal/terminal.cpp
    api/time/chronoMs.cpp
    api/time/periodicCallsMs.cpp
    api/time/timeMs.cpp

    led/colorOrder.cpp
    led/ledManager.cpp
    led/rgbColor.cpp
    led/rgbLed.cpp

    maths/matrix/matrix.cpp

    micro/micro.cpp
    micro/microManager.cpp
    micro/learning/impact.cpp
    micro/learning/impactsManager.cpp
    micro/learning/learning.cpp
    micro/learning/record.cpp
    micro/learning/recordSlot.cpp

    modules/module.cpp
    modules/moduleManager.cpp

    network/client/client.cpp
    network/client/ipv4.cpp
    network/client/macAddr.cpp
    network/udp/udp.cpp
    network/udp/udpPacket.cpp
    network/udpParser/udpParser.cpp

    session/session.cpp
    )

include_directories(
    animation
    api/terminal
    api/time
    led
    maths/matrix
    micro
    micro/learning
    modules
    network
    network/client
    network/udp
    network/udpParser
    session
)
add_executable(MasterProject ${SRCS_FILES})

target_link_libraries(${CMAKE_PROJECT_NAME} Ws2_32)
target_link_libraries(${CMAKE_PROJECT_NAME} Mswsock)
target_link_libraries(${CMAKE_PROJECT_NAME} AdvApi32)
target_link_libraries(${CMAKE_PROJECT_NAME} iphlpapi)

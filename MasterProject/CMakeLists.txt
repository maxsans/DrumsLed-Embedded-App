cmake_minimum_required(VERSION 3.5.0)
project(MasterProject VERSION 0.1.0 LANGUAGES C CXX)

set(SRCS_FILES
    main.cpp

    animation/animation.cpp
    animation/animationManager.cpp
    animation/fade.cpp
    animation/simpleColor.cpp

    api/terminal/terminal.cpp
    api/udp/udp.cpp
    api/udp/udpPacket.cpp

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

    session/session.cpp

    udpParser/udpParser.cpp
    )

include_directories(
    animation
    api/terminal
    api/udp
    led
    maths/matrix
    micro
    micro/learning
    modules
    session
    udpParser
)
add_executable(MasterProject ${SRCS_FILES})

target_link_libraries(${CMAKE_PROJECT_NAME} Ws2_32)
target_link_libraries(${CMAKE_PROJECT_NAME} Mswsock)
target_link_libraries(${CMAKE_PROJECT_NAME} AdvApi32)

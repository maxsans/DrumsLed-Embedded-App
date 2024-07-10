cmake_minimum_required(VERSION 3.5.0)
project(MasterProject VERSION 0.1.0 LANGUAGES C CXX)

set(SRCS_FILES
    main.cpp

    api/terminal/terminal.cpp
    api/udp/udp.cpp
    api/udp/udpPacket.cpp

    led/rgbColor.cpp
    led/rgbLed.cpp
    led/ledManager.cpp

    maths/coeff/coeff.cpp
    maths/matrix/matrix.cpp

    micro/impact.cpp
    micro/impactsManager.cpp
    micro/learning.cpp
    micro/micro.cpp
    micro/microManager.cpp

    modules/module.cpp
    modules/moduleManager.cpp

    session/session.cpp

    udpParser/udpParser.cpp
    )

include_directories(
    api/terminal
    api/udp
    led
    maths/coeff
    maths/matrix
    micro
    modules
    session
    udpParser
)
add_executable(MasterProject ${SRCS_FILES})

target_link_libraries(${CMAKE_PROJECT_NAME} Ws2_32)
target_link_libraries(${CMAKE_PROJECT_NAME} Mswsock)
target_link_libraries(${CMAKE_PROJECT_NAME} AdvApi32)

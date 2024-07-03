cmake_minimum_required(VERSION 3.5.0)
project(MasterProject VERSION 0.1.0 LANGUAGES C CXX)

set(SRCS_FILES
    main.cpp
    api/udp/udp.cpp)

include_directories(api)
include_directories(api/udp)

add_executable(MasterProject ${SRCS_FILES})

target_link_libraries(${CMAKE_PROJECT_NAME} Ws2_32)
target_link_libraries(${CMAKE_PROJECT_NAME} Mswsock)
target_link_libraries(${CMAKE_PROJECT_NAME} AdvApi32)

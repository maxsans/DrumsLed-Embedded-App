
add_sources(
    api/logs/logsWindows.cpp
    api/udp/udpWindows.cpp
    api/time/timeWindows.cpp
    api/tcp/tcpWindows.cpp
    api/terminal/terminalWindows.cpp
    api/mutex/mutexWindows.cpp
)

add_libraries(
    ws2_32
    iphlpapi
)

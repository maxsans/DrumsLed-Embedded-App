
add_sources(
    api/logs/logsLinux.cpp
    api/udp/udpLinux.cpp
    api/tcp/tcpLinux.cpp
    api/time/timeLinux.cpp
    api/terminal/terminalLinux.cpp
)

add_libraries(
    pthread
)

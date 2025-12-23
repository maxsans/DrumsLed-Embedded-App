
add_sources(
    api/logs/logsLinux.cpp
    api/network/udp/udpLinux.cpp
    api/network/tcp/tcpLinux.cpp
    api/network/networkConversion/networkConversionLinux.cpp
    api/time/timeLinux.cpp
    api/terminal/terminalLinux.cpp
    api/mutex/mutexLinux.cpp
    api/file/fileLinux.cpp
)

add_libraries(
    pthread
)

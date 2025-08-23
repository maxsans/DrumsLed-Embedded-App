
# Add .exe to the target name
set(TARGET ${TARGET}.exe)

add_sources(
    api/logs/logsWindows.cpp
    api/network/udp/udpWindows.cpp
    api/network/tcp/tcpWindows.cpp
    api/network/networkConversion/networkConversionWindows.cpp
    api/time/timeWindows.cpp
    api/terminal/terminalWindows.cpp
    api/mutex/mutexWindows.cpp
    api/file/fileWindows.cpp
)

add_libraries(
    ws2_32
    iphlpapi
    shlwapi
)

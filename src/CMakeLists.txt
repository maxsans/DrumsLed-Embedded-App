## CMakeLists.txt used only for esp-idf build system

idf_component_register(SRCS ${SRCS}
    INCLUDE_DIRS ${INCLUDE_DIRS}
    REQUIRES "freertos" "esp8266" "spi_flash" "pthread" "app_update" "bootloader_support"
)

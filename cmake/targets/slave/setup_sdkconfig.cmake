# Function to setup sdkconfig for ESP8266 builds
function(setup_sdkconfig_for_esp8266)
    set(SDKCONFIG_PATH "${CMAKE_SOURCE_DIR}/sdkconfig")
    set(SDKCONFIG_DEFAULT_PATH "${CMAKE_SOURCE_DIR}/sdkconfig.default")
    
    # If sdkconfig doesn't exist or is older than sdkconfig.default, update it
    if(NOT EXISTS "${SDKCONFIG_PATH}" OR 
       "${SDKCONFIG_DEFAULT_PATH}" IS_NEWER_THAN "${SDKCONFIG_PATH}")
        
        if(EXISTS "${SDKCONFIG_DEFAULT_PATH}")
            message(STATUS "Setting up sdkconfig from sdkconfig.default")
            configure_file("${SDKCONFIG_DEFAULT_PATH}" "${SDKCONFIG_PATH}" COPYONLY)
        else()
            message(WARNING "sdkconfig.default not found at ${SDKCONFIG_DEFAULT_PATH}")
        endif()
    endif()
endfunction()

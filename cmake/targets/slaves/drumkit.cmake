
# Preprocessor definitions
add_compile_options(-D__TARGET_DRUMKIT)

####################################################################################################
# DrumKit source files (from src directory)
####################################################################################################
add_sources(
    api/adc/adcEsp8266.cpp
    api/addrLed/addrLedEsp8266.cpp
    slaves/drumKit/drumKit.cpp
    )

####################################################################################################
# DrumKit include directories (from src directory)
####################################################################################################
add_include_dirs(
    )

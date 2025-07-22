
# Preprocessor definitions
add_compile_options(-D__TARGET_DRUMKIT)

####################################################################################################
# DrumKit source files (from src directory)
####################################################################################################
add_sources(
    api/adc/adcEsp8266.cpp
    api/circleLeds/circleLedsDrumKit.cpp
    slaves/drumKit/drumKit.cpp
    kit/kitConfigGenerator/kitConfigGeneratorDrumKit.cpp
    )

####################################################################################################
# DrumKit include directories (from src directory)
####################################################################################################
add_include_dirs(
    )

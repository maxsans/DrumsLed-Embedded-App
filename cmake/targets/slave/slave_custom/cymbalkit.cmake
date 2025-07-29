
# Preprocessor definitions
add_compile_options(-D__TARGET_CYMBALKIT)

####################################################################################################
# CymbalKit source files (from src directory)
####################################################################################################
add_sources(
    api/adc/adcEsp8266.cpp
    api/circleLeds/circleLedsCymbalKit.cpp
    kit/kitConfigGenerator/kitConfigGeneratorCymbalKit.cpp
    )

####################################################################################################
# CymbalKit include directories (from src directory)
####################################################################################################
add_include_dirs(
    )

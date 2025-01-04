
include(cmake/tools.cmake)

# Preprocessor definitions
add_compile_options(-D__TARGET_MASTER)

####################################################################################################
# Master source files (from src directory)
####################################################################################################
add_sources(
    master/masterLaunch.cpp
    )

####################################################################################################
# Master include directories (from src directory)
####################################################################################################
add_include_dirs(
    master
    )

# Print the sources and include directories
print_sources_and_include_dirs()

add_executable(${TARGET} ${SRCS})
target_include_directories(${TARGET} PRIVATE ${INCLUDE_DIRS})

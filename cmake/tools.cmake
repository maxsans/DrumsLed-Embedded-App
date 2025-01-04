## Functions and macros to help with the CMake files

# Macro to add some sources files to the list of sources (SRCS)
# Don't forget to set the SRC_PREFIX variable before using this macro
# if SRCS is not defined, it will be created with a global scope
macro(add_sources)
    foreach(src ${ARGN})
        list(APPEND SRCS "${SRC_PREFIX}/${src}")
    endforeach()
endmacro()

# Macro to add some include directories to the list of include directories (INCLUDE_DIRS)
# Don't forget to set the SRC_PREFIX variable before using this macro
# if INCLUDE_DIRS is not defined, it will be created with a global scope
macro(add_include_dirs)
    foreach(dir ${ARGN})
        list(APPEND INCLUDE_DIRS "${SRC_PREFIX}/${dir}")
    endforeach()
endmacro()

# Macro to print the list of sources and include directories
macro(print_sources_and_include_dirs)
    message(STATUS "Sources:")
    foreach(src ${SRCS})
        message(STATUS "    ${src}")
    endforeach()
    message(STATUS "Include directories:")
    foreach(dir ${INCLUDE_DIRS})
        message(STATUS "    ${dir}")
    endforeach()
endmacro()

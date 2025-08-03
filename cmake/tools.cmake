## Functions and macros to help with the CMake files

# Macro to add some sources files to the list of sources (SRCS)
# Don't forget to set the SRC_DIR variable before using this macro
# if SRCS is not defined, it will be created with a global scope
macro(add_sources)
    if(NOT DEFINED SRCS)
        set(SRCS "" CACHE INTERNAL "List of source files")
    endif()
    foreach(src ${ARGN})
        list(APPEND SRCS "${SRC_DIR}/${src}")
    endforeach()
endmacro()

# Macro to add some include directories to the list of include directories (INCLUDE_DIRS)
# Don't forget to set the SRC_DIR variable before using this macro
# if INCLUDE_DIRS is not defined, it will be created with a global scope
macro(add_include_dirs)
    if(NOT DEFINED INCLUDE_DIRS)
        set(INCLUDE_DIRS "" CACHE INTERNAL "List of include directories")
    endif()
    foreach(dir ${ARGN})
        list(APPEND INCLUDE_DIRS "${SRC_DIR}/${dir}")
    endforeach()
endmacro()

# Macro to add a library to the list of libraries (LIBS)
# if LIBS is not defined, it will be created with a global scope
macro(add_libraries)
    if(NOT DEFINED LIBS)
        set(LIBS "" CACHE INTERNAL "List of libraries")
    endif()
    foreach(lib ${ARGN})
        list(APPEND LIBS "${lib}")
    endforeach()
endmacro()

# Macro to print the list of sources, the list of include directories
# and the list of libraries
macro(print_app_info)
    message(STATUS "Sources:")
    foreach(src ${SRCS})
        message(STATUS "    ${src}")
    endforeach()
    message(STATUS "Include directories:")
    foreach(dir ${INCLUDE_DIRS})
        message(STATUS "    ${dir}")
    endforeach()
    message(STATUS "Libraries:")
    foreach(lib ${LIBS})
        message(STATUS "    ${lib}")
    endforeach()
endmacro()

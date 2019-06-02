## CMakeLists.txt
## Stanisław Grams <sgrams@fmdx.pl>
##
## secpass.org
## © 2019
cmake_minimum_required (VERSION 2.8.11)
project (secpass)

set (PACKAGE ${PROJECT_NAME})

if (CMAKE_COMPILER_IS_GNUCXX OR CMAKE_COMPILER_IS_CLANGXX)
    add_definitions (-Wall)
    add_definitions (-Wextra)
    add_definitions (-Wno-unused-parameter)
    add_definitions (-Wsign-compare)
endif ()

set (SGX_SDK $ENV{SGX_SDK})
set (SGX_ARCH x64)
set (SGX_MODE SIM CACHE STRING "SGX mode: SIM or HW") # SGX mode: sim, hw
set (COMPILE_EXAMPLES NO CACHE BOOL "Build examples")
set (SGX_BUILD DEBUG CACHE STRING "Build: DEBUG, PRERELEASE or RELEASE")

include (cmake/sgx_init.cmake)

# Tell CMake to run moc when necessary
#set (CMAKE_AUTOMOC ON)
#set (CMAKE_INCLUDE_CURRENT_DIR ON)

# Gather all src files from subdirs
function (add_src_files list)
    get_property (is_defined GLOBAL PROPERTY SRC_LIST DEFINED)
    if (NOT is_defined)
        define_property (GLOBAL PROPERTY ${list}
            BRIEF_DOCS "List of source files"
            FULL_DOCS "List of source files to be compiled in one library")
    endif ()
    # make absolute paths
    set (SRC)
    foreach (s IN LISTS ARGN)
        if (NOT IS_ABSOLUTE "${s}")
            get_filename_component (s "${s}" ABSOLUTE)
        endif ()
        list (APPEND SRC "${s}")
    endforeach ()
    # append to global list
    set_property (GLOBAL APPEND PROPERTY ${list} "${SRC}")
endfunction (add_src_files)

add_subdirectory (src)
#add_subdirectory (test)

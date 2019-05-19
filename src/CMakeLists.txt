## src/CMakeLists.txt
## Stanisław Grams <sgrams@fmdx.pl>
##
## secpass.org
## © 2019
# add src subdirs
add_subdirectory (qtgui)
add_subdirectory (nctui)

# SGX related settings and checks
enable_language (ASM)
if (NOT DEFINED ENV{SGX_SDK})
  message (FATAL_ERROR "$SGX_SDK environment variable must be set up.")
endif ()


if (CMAKE_SIZEOF_VOID_P EQUAL 4)
  set (CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -m32")
  set (CMAKE_LIBRARY_PATH "${CMAKE_LIBRARY_PATH} $ENV{SGX_SDK}/lib")
  set (SGX_ENCLAVE_SIGNER "$ENV{SGX_SDK}/bin/x86/sgx_sign")
  set (SGX_EDGER8R "$ENV{SGX_SDK}/bin/x86/sgx_edger8r")
else ()
  set (CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -m64")
  set (CMAKE_LIBRARY_PATH ${CMAKE_LIBRARY_PATH} "$ENV{SGX_SDK}/lib64")
  set (SGX_ENCLAVE_SIGNER "$ENV{SGX_SDK}/bin/x64/sgx_sign")
  set (SGX_EDGER8R "$ENV{SGX_SDK}/bin/x64/sgx_edger8r")
endif ()


# set and check qt
find_package (Qt5 COMPONENTS Core Network Widgets Svg)
get_property (${PROJECT_NAME}_QT_SRC GLOBAL PROPERTY QT_SRC_LIST)
get_property (${PROJECT_NAME}_QT_UI_SRC GLOBAL PROPERTY QT_UI_SRC_LIST)

# process qtgui files
QT5_WRAP_UI (QT_UIS_HDRS
  ${${PROJECT_NAME}_QT_UI_SRC}
  )
# process qtgui resources
QT5_ADD_RESOURCES (ASSETS_LIST
  ../assets/icons.qrc
  ../assets/logo.qrc
  )

# if set and check nc properties
set (CURSES_NEED_NCURSES TRUE)
find_package (Curses REQUIRED)
get_property (${PROJECT_NAME}_NC_SRC GLOBAL PROPERTY NC_SRC_LIST)

# build program
## qt gui
add_executable (${PROJECT_NAME}-qt ${${PROJECT_NAME}_QT_SRC} ${QT_UIS_HDRS} ${ASSETS_LIST})
set_property (TARGET ${PROJECT_NAME}-qt PROPERTY CXX_STANDARD 11)
target_link_libraries(${PROJECT_NAME}-qt
  Qt5::Core
  Qt5::Widgets
  Qt5::Svg
  )

## ncurses tui
add_executable (${PROJECT_NAME}-nc ${${PROJECT_NAME}_NC_SRC})
set_property (TARGET ${PROJECT_NAME}-nc PROPERTY CXX_STANDARD 11)
target_link_libraries(${PROJECT_NAME}-nc
  ${CURSES_LIBRARIES}
  )

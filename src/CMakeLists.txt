## src/CMakeLists.txt
## Stanisław Grams <sgrams@fmdx.pl>
##
## secpass.org
## © 2019
list (APPEND CMAKE_MODULE_PATH cmake)
find_package (SGX REQUIRED)
set (CMAKE_CXX_STANDARD 11)
# add src subdirs
add_subdirectory (core)
add_subdirectory (qtgui)
add_subdirectory (nctui)

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

get_property (${PROJECT_NAME}_CORE_SRC GLOBAL PROPERTY CORE_SRC_LIST)
# build program
## core
add_enclave_library (
  core
  SRCS ${CORE_SRC_LIST}
  EDL core.edl
  EDL_SEARCH_PATHS core
  )
enclave_sign (
  core
  KEY core_private.pem
  CONFIG core.config.xml
  )
## qt gui
#add_executable (${PROJECT_NAME}-qt ${${PROJECT_NAME}_QT_SRC} ${QT_UIS_HDRS} ${ASSETS_LIST})
#set_property (TARGET ${PROJECT_NAME}-qt PROPERTY CXX_STANDARD 11)
#target_link_libraries(${PROJECT_NAME}-qt
#  Qt5::Core
#  Qt5::Widgets
#  Qt5::Svg
#  )

## ncurses tui
set_source_files_properties (
        ${CMAKE_CURRENT_SOURCE_DIR}/../core/core_u.c
        PROPERTIES GENERATED TRUE
)
add_executable (${PROJECT_NAME}-nc ${${PROJECT_NAME}_NC_SRC} ${CMAKE_CURRENT_SOURCE_DIR}/../core/core_u.c)
set_property (TARGET ${PROJECT_NAME}-nc PROPERTY CXX_STANDARD 11)
target_link_libraries(${PROJECT_NAME}-nc
  ${CURSES_LIBRARIES}
  )

## src/CMakeLists.txt
## Stanisław Grams <sgrams@fmdx.pl>
##
## secpass.org
## © 2019
# add src subdirs
add_subdirectory (qtgui)
add_subdirectory (nctui)

# set and check qt
find_package (Qt5 COMPONENTS Core Network Widgets Svg)
get_property (${PROJECT_NAME}_QT_SRC GLOBAL PROPERTY QT_SRC_LIST)
get_property (${PROJECT_NAME}_QT_UI_SRC GLOBAL PROPERTY QT_UI_SRC_LIST)

# process qtgui files
QT5_WRAP_UI (QT_UIS_HDRS
  ${${PROJECT_NAME}_QT_UI_SRC}
  )
# process qtgui resources
#QT5_ADD_RESOURCES (ASSETS_LIST
#  ../assets/file1.qrc
#  )

# if set and check nc properties
set (CURSES_NEED_NCURSES TRUE)
find_package (Curses REQUIRED)
get_property (${PROJECT_NAME}_NC_SRC GLOBAL PROPERTY NC_SRC_LIST)

# build program
## qt gui
add_executable (${PROJECT_NAME}-qt ${${PROJECT_NAME}_QT_SRC} ${QT_UIS_HDRS})
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

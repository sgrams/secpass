##
## Stanisław Grams <sgrams@fmdx.pl>
## src/CMakeLists.txt
##
## secpass-git
##
# add src subdirs
add_subdirectory (qtgui)

# set global properties
get_property (${PROJECT_NAME}_SRC GLOBAL PROPERTY SRC_LIST)
get_property (${PROJECT_NAME}_UI_SRC GLOBAL PROPERTY UI_SRC_LIST)

# process qtgui files
QT5_WRAP_UI (UIS_HDRS
  ${${PROJECT_NAME}_UI_SRC}
  )
# process resources
#QT5_ADD_RESOURCES (ASSETS_LIST
#  ../assets/file1.qrc
#  )

# build program
#add_executable (${PROJECT_NAME} ${${PROJECT_NAME}_SRC} ${UIS_HDRS} ${ASSETS_LIST})
add_executable (${PROJECT_NAME} ${${PROJECT_NAME}_SRC} ${UIS_HDRS})
set_property (TARGET ${PROJECT_NAME} PROPERTY CXX_STANDARD 11)

target_link_libraries(${PROJECT_NAME}
  Qt5::Core
  Qt5::Widgets
)

set (INSTALL_DEFAULT_BINDIR "bin" CACHE STRING "Appended to CMAKE_INSTALL_PREFIX")
install (TARGETS ${PROJECT_NAME} RUNTIME DESTINATION ${INSTALL_DEFAULT_BINDIR})

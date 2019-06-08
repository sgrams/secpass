## src/qtgui/CMakeLists.txt
## Stanisław Grams <sgrams@fmdx.pl>
##
## secpass.org
## © 2019
# check Qt and SGX dependencies
find_package (Qt5 COMPONENTS Core Network Widgets Svg)
get_property (${PROJECT_NAME}_QT_SRC GLOBAL PROPERTY QT_SRC_LIST)
get_property (${PROJECT_NAME}_QT_UI_SRC GLOBAL PROPERTY QT_UI_SRC_LIST)
set (CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${SGX_COMMON_CFLAGS} -fPIC -Wno-attributes")
set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${CMAKE_C_FLAGS}")
set (CMAKE_AUTOMOC ON)
set (CMAKE_AUTOUIC "YES")
set (CMAKE_INCLUDE_CURRENT_DIR ON)

# set DEBUG flags
set (CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -O0")
set (CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -O0")

# include src dirs
include_directories (
  $(CMAKE_CURRENT_SOURCE_DIR)
  ${SGX_SDK}/include
  )

# mock core_u.c from core/ (untrusted part of the bridge)
set_source_files_properties (
  ${CMAKE_CURRENT_SOURCE_DIR}/../core/core_u.c
  PROPERTIES GENERATED TRUE
  )

# process Qt files
add_src_files (QT_SRC_LIST
  main.cc
  mainwindow.cc
  mainwindow.h
  aboutdialog.cc
  aboutdialog.h
  settingsdialog.cc
  settingsdialog.h
  ${CMAKE_CURRENT_SOURCE_DIR}/../core/core_u.c
  ${CMAKE_CURRENT_SOURCE_DIR}/../common/bridge.cc
  )

add_src_files (QT_UI_SRC_LIST
  mainwindow.ui
  aboutdialog.ui
  settingsdialog.ui
  )

QT5_WRAP_UI (QT_UIS_HDRS
  ${${PROJECT_NAME}_QT_UI_SRC}
  )

QT5_ADD_RESOURCES (ASSETS_LIST
  ../../assets/icons.qrc
  ../../assets/logo.qrc
  )
get_property (${PROJECT_NAME}_QT_SRC GLOBAL PROPERTY QT_SRC_LIST)
get_property (${PROJECT_NAME}_QT_UI_SRC GLOBAL PROPERTY QT_UI_SRC_LIST)

add_executable (
  secpass-qt
  ${${PROJECT_NAME}_QT_SRC}
  ${QT_UIS_HDRS}
  ${ASSETS_LIST}
  )
target_link_libraries (
  secpass-qt
  Qt5::Core
  Qt5::Widgets
  Qt5::Svg
  -L${SGX_LIBRARY_PATH} ${SGX_URTS_LIB} ${SGX_USVC_LIB}
  )

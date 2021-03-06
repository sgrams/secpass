## src/nctui/CMakeLists.txt
## Stanisław Grams <sgrams@fmdx.pl>
##
## secpass.org
## © 2019
# check and set libncurses and SGX dependencies
find_package(PkgConfig REQUIRED)
set (CURSES_NEED_NCURSES TRUE)
find_package (Curses REQUIRED)
get_property (${PROJECT_NAME}_NC_SRC GLOBAL PROPERTY NC_SRC_LIST)
get_property (${PROJECT_NAME}_CORE_SRC GLOBAL PROPERTY CORE_SRC_LIST)
set (CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${SGX_COMMON_CFLAGS} -fPIC -Wno-attributes")
set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${CMAKE_C_FLAGS}")

# set DEBUG flags
set (CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -O0")
set (CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -O0")

# check for Argon2 KDF & OpenSSL
find_package (Argon2 REQUIRED)
find_package (OpenSSL REQUIRED)

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

# process nc files
add_src_files (
  NC_SRC_LIST
  main.cc
  draw.cc
  window.cc
  ui.cc
  clipboard.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/../core/core_u.c
  ${CMAKE_CURRENT_SOURCE_DIR}/../common/bridge.cc

  # headers
  draw.h
  window.h
  ui.h
  clipboard.h
  )

get_property (${PROJECT_NAME}_NC_SRC GLOBAL PROPERTY NC_SRC_LIST)

# perform linking
link_directories (
  .
  )

link_libraries (
  c
  stdc++
  ncurses
  ${ARGON2_LIBRARIES}
  ${OPENSSL_LIBRARIES}
  -L${SGX_LIBRARY_PATH} ${SGX_URTS_LIB} ${SGX_USVC_LIB}
  )

add_executable (
  secpass-nc
  ${${PROJECT_NAME}_NC_SRC}
  )

add_dependencies (
  secpass-nc
  core
  )

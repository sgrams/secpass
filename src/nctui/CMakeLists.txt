## src/nctui/CMakeLists.txt
## Stanisław Grams <sgrams@fmdx.pl>
##
## secpass.org
## © 2019
set (CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${SGX_COMMON_CFLAGS} -fPIC -Wno-attributes")
set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${CMAKE_C_FLAGS}")

include_directories (
  $(CMAKE_CURRENT_SOURCE_DIR)
  ${SGX_SDK}/include
  )

set (CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -O0")
set (CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -O0")

link_directories (.)
link_libraries (
  c
  stdc++
  ncurses
  -L${SGX_LIBRARY_PATH} ${SGX_URTS_LIB} ${SGX_USVC_LIB}
  )

set_source_files_properties (
  ${CMAKE_CURRENT_SOURCE_DIR}/../core/core_u.c
  PROPERTIES GENERATED TRUE
  )

set (
  SOURCES
  ${CMAKE_CURRENT_SOURCE_DIR}/../core/core_u.c
  )

add_executable (secpass-nc main.cc ${SOURCES})

add_dependencies (
  secpass-nc
  core
  )

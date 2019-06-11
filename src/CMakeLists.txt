## src/CMakeLists.txt
## Stanisław Grams <sgrams@fmdx.pl>
##
## secpass.org
## © 2019

list (APPEND CMAKE_MODULE_PATH cmake)
find_package (SGX REQUIRED)
find_package (Argon2 REQUIRED)
find_package (OpenSSL REQUIRED)
set (CMAKE_CXX_STANDARD 11)

# add src subdirs
add_subdirectory (core)
add_subdirectory (nctui)

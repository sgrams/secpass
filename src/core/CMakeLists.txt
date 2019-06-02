## src/core/CMakeLists.txt
## Stanisław Grams <sgrams@fmdx.pl>
##
## secpass.org
## © 2019
add_custom_command (
  OUTPUT ${CMAKE_CURRENT_SOURCE_DIR}/core_t.c
  DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/core.edl
  COMMAND ${SGX_EDGER8R} --trusted ${CMAKE_CURRENT_SOURCE_DIR}/core.edl
  --search-path ${TC_ROOT_DIR}/Enclave
  --search-path ${SGX_SDK}/include
  WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
  )

add_custom_command (
  OUTPUT ${CMAKE_CURRENT_SOURCE_DIR}/core_u.h
  DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/core.edl
  COMMAND ${SGX_EDGER8R} --untrusted ${CMAKE_CURRENT_SOURCE_DIR}/core.edl
  --search-path ${TC_ROOT_DIR}/Enclave
  --search-path ${SGX_SDK}/include
  WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
  )

include_directories (
  $(CMAKE_CURRENT_SOURCE_DIR)
  ${SGX_SDK}/include
  ${SGX_SDK}/include/tlibc
  ${SGX_SDK}/include/stlport
  ${SGX_SDK}/include/libcxx
  .
  )

add_library (core SHARED
  core.cc
  core.h
  random.cc
  random.h
  crypto.cc
  crypto.h
  core_t.c
  core_u.h
  )

set (CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${SGX_COMMON_CFLAGS} -nostdinc -fvisibility=hidden -fpie -fstack-protector")
set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${CMAKE_C_FLAGS} -nostdinc++ -DENCLAVE")

target_link_libraries (core PRIVATE "${SGX_COMMON_CFLAGS}\
  -Wl,--no-undefined -nostdlib -nodefaultlibs -Wl,--no-undefined -nostdlib -nodefaultlibs -nostartfiles -L${SGX_LIBRARY_PATH} \
  -Wl,--whole-archive -l${SGX_TRTS_LIB} -Wl,--no-whole-archive \
  -Wl,--start-group -lsgx_tstdc -lsgx_tcxx -lsgx_tkey_exchange -lsgx_tcrypto -l${SGX_TSVC_LIB} -Wl,--end-group \
  -Wl,-Bstatic -Wl,-Bsymbolic -Wl,--no-undefined \
  -Wl,-pie,-eenclave_entry -Wl,--export-dynamic  \
  -Wl,--defsym,__ImageBase=0 -Wl,--gc-sections   \
  -L${CMAKE_CURRENT_SOURCE_DIR}")

set (CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/../../lib/)
add_custom_command (TARGET core
  POST_BUILD
  COMMAND ${SGX_ENCLAVE_SIGNER} sign
  -key core_private.pem
  -config core.config.xml
  -enclave $<TARGET_FILE:core>
  -out ${CMAKE_SOURCE_DIR}/lib/enclave.so
  WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
  COMMENT "Signing the core enclave => enclave.so")

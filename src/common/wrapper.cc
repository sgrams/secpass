/* src/common/wrapper.cc
 * Stanisław Grams <sgrams@fmdx.pl>
 *
 * secpass.org
 * © 2019
 */
#include "wrapper.h"

sgx_enclave_id_t global_eid = 0;

int initialize_enclave (void)
{
  sgx_status_t rv = SGX_ERROR_UNEXPECTED;

  rv = sgx_create_enclave("../lib/enclave.so", 1, NULL, NULL, &global_eid, NULL);
  if (rv == SGX_SUCCESS) {
    return 0;
  }
  rv = sgx_create_enclave("/usr/share/libsecpass/enclave.so", 1, NULL, NULL, &global_eid, NULL);
  if (rv != SGX_SUCCESS) {
    return -1;
  }
  return 0;
}
int destroy_enclave (void)
{
  if (SGX_SUCCESS != sgx_destroy_enclave (global_eid)) {
    return -1;
  }
  return 0;
}

/* src/common/wrapper.cc
 * Stanisław Grams <sgrams@fmdx.pl>
 *
 * secpass.org
 * © 2019
 */
#include "wrapper.h"
#include <vector>

using namespace std;

sgx_enclave_id_t global_eid = 0;
bool authenticated = false;
vector<string> names;

wrapper_status_t
initialize_enclave (void)
{
  if (SGX_SUCCESS != sgx_create_enclave("../lib/enclave.so", 1, NULL, NULL, &global_eid, NULL)) {
    return WRAP_ER_INIT;
  }
  return WRAP_OK;
}

wrapper_status_t
destroy_enclave (void)
{
  if (SGX_SUCCESS != sgx_destroy_enclave (global_eid)) {
    return WRAP_ER_DESTROY;
  }
  return WRAP_OK;
}


wrapper_status_t
secret_add (string name, string secret)
{
  uint64_t rv;
  uint8_t  sc;
  wrapper_status_t status = WRAP_OK;
  secret_check (name, &sc);
  if (!sc) {
    return WRAP_ER_UNDEF;
  }

  const char *str = name.c_str ();
  char       *sec = new char[secret.length () + 1];
  strcpy (sec, secret.c_str ());

  if (SGX_SUCCESS != entry_add (global_eid, &rv, str, sec)) {
    return WRAP_ER_ADD;
  }
  names.push_back (name);

  delete[] sec;
  return status;
}

wrapper_status_t
secret_set (char *name, size_t name_len, char *secret, size_t secret_len)
{
  wrapper_status_t status = WRAP_OK;

  return status;
}

wrapper_status_t
secret_del (string name)
{
  uint64_t rv;
  wrapper_status_t status = WRAP_OK;

  const char *str = name.c_str ();
  if (SGX_SUCCESS != entry_del (global_eid, &rv, str)) {
    return WRAP_ER_DEL;
  }

  return status;
}

wrapper_status_t
secret_check (string name, uint8_t *rv) {
  uint64_t rs;
  const char *str = name.c_str ();

  if (SGX_SUCCESS != entry_check (global_eid, &rs, str, rv)) {
    return WRAP_ER_UNDEF;
  }
  return rs;
}

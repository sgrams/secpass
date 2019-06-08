/* src/common/bridge.cc
 * Stanisław Grams <sgrams@fmdx.pl>
 *
 * secpass.org
 * © 2019
 */
#include "bridge.h"
#include <vector>
#include <algorithm>

using namespace std;

sgx_enclave_id_t global_eid = 0;
bool authenticated = false;
vector<string> names;

bridge_status_t
br_enclave_init (void)
{
  if (SGX_SUCCESS != sgx_create_enclave("../lib/enclave.so", 1, NULL, NULL, &global_eid, NULL)) {
    return BRIDGE_ER_INIT;
  }
  return BRIDGE_OK;
}

bridge_status_t
br_enclave_close (void)
{
  if (SGX_SUCCESS != sgx_destroy_enclave (global_eid)) {
    return BRIDGE_ER_DESTROY;
  }
  return BRIDGE_OK;
}


bridge_status_t
br_secret_add (string name, string secret)
{
  uint64_t rv;
  uint8_t  sc;
  bridge_status_t status = BRIDGE_OK;
  br_secret_check (name, &sc);
  if (!sc) {
    return BRIDGE_ER_UNDEF;
  }

  const char *str = name.c_str ();
  char       *sec = new char[secret.length () + 1];
  strcpy (sec, secret.c_str ());

  if (SGX_SUCCESS != entry_add (global_eid, &rv, str, sec)) {
    return BRIDGE_ER_ADD;
  }
  names.push_back (name);

  delete[] sec;
  return status;
}

bridge_status_t
br_secret_set (char *name, size_t name_len, char *secret, size_t secret_len)
{
  bridge_status_t status = BRIDGE_OK;

  return status;
}

bridge_status_t
br_secret_del (string name)
{
  uint64_t rv;
  bridge_status_t status = BRIDGE_OK;

  const char *str = name.c_str ();
  if (SGX_SUCCESS != entry_del (global_eid, &rv, str)) {
    return BRIDGE_ER_DEL;
  }
  auto it = find (names.begin (), names.end (), name);
  if (it != names.end ()) {
    names.erase (it);
  }

  return status;
}

bridge_status_t
br_secret_check (string name, uint8_t *rv) {
  uint64_t rs;
  const char *str = name.c_str ();

  if (SGX_SUCCESS != entry_check (global_eid, &rs, str, rv)) {
    return BRIDGE_ER_UNDEF;
  }
  return rs;
}

bridge_status_t
br_secret_fetch (string name, string *secret)
{
  uint64_t rv;
  const char *str = name.c_str ();
  char buf[MAX_SECRET_LEN];

  if (SGX_SUCCESS != entry_find (global_eid, &rv, str, buf)) {
    return BRIDGE_ER_FETCH;
  }

  *secret = string (buf);
  memset (buf, 0, sizeof (buf));
  return rv;
}

bridge_status_t
br_fetch_names (vector<string> *rv)
{
  bridge_status_t status = BRIDGE_OK;
  *rv = names;
  return status;
}

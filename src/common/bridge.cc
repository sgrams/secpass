/* src/common/bridge.cc
 * Stanisław Grams <sgrams@fmdx.pl>
 *
 * secpass.org
 * © 2019
 */
#include "bridge.h"
#include "file.h"
#include "argon2.h"
#include "../core/crypto.h"

#include <sys/stat.h>
#include <vector>
#include <algorithm>

using namespace std;

sgx_enclave_id_t global_eid = 0;
bool authenticated = false;
vector<string> names;
key_128bit_t key;

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
  char sec[MAX_SECRET_LEN] = {0};
  strncpy (sec, secret.c_str (), secret.size () + 1);

  if (SGX_SUCCESS != entry_add (global_eid, &rv, str, sec)) {
    return BRIDGE_ER_ADD;
  }
  names.push_back (name);

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
br_secret_fetch (string name, char *secret)
{
  uint64_t rv;
  const char *str = name.c_str ();

  if (SGX_SUCCESS != fetch_decrypted_secret (global_eid, &rv, str, secret)) {
    return BRIDGE_ER_FETCH;
  }

  return rv;
}

bridge_status_t
br_fetch_names (vector<string> *rv)
{
  bridge_status_t status = BRIDGE_OK;
  *rv = names;
  return status;
}

// file-specific methods
bridge_status_t
br_file_check (string filepath)
{
  bridge_status_t status = BRIDGE_OK;

  if (filepath.substr (filepath.find_last_of (".") + 1) != DEFAULT_FILE_EXTENSION) {
    status = BRIDGE_ER_WR_EXT;
    return status;
  }

  return br_file_exists (filepath);
}

bridge_status_t
br_file_exists (string filepath)
{
  bridge_status_t status = BRIDGE_OK;
  struct stat buffer;

  if (stat (filepath.c_str (), &buffer) != 0) {
    status = BRIDGE_ER_WR_FILE;
  }

  return status;
}

bridge_status_t
br_file_open (string master_key, string filepath)
{
  uint64_t rv;
  key_128bit_t mem_key;

  //memcpy (mem_key, master_key, 16); // 128-bit GCM key
  memcpy (mem_key, "C1DA44CBC3CE421", 16); // 128-bit GCM key

  bridge_status_t status = BRIDGE_OK;
  ifstream in;
  json   data;

  in.open (filepath, ios::binary);
  in >> data;

  string  name;
  uint8_t secret[MAX_SECRET_SIZ] = {0};

  if (SGX_SUCCESS != auth (global_eid, &rv, mem_key)) {
    status = BRIDGE_ER_INIT;
    return status;
  }
  memset (mem_key, 0, 16);

  // TODO: read kdf, enc and version from file
  for (auto &el : data["entries"].items ())
  {
    name = el.key ();
    names.push_back (name);
    auto &val = el.value ();

    val.get_to (secret);

    if (SGX_SUCCESS != load_to_sec_map (global_eid, &rv, name.c_str (), (char *)secret))
    {
      status = -1;
      return status;
    }
  }

  return status;
}

bridge_status_t
br_file_save (string filepath)
{
  bridge_status_t status = BRIDGE_OK;
  ofstream out;
  json data;

  out.open (filepath, ios::binary | ios::out);

  // set file json data
  data["version"] = SECPASS_FILE_VERSION;
  data["kdf"] = KDF_ARGON2;
  data["enc"] = ENC_AES128GCM;

  uint64_t rv;
  uint8_t  secret[MAX_SECRET_SIZ] = {0};

  for (auto &el : names)
  {
    if (SGX_SUCCESS != fetch_encrypted_secret (global_eid, &rv, el.c_str (), secret)) {
      return BRIDGE_ER_WR_FILE;
    }

    data["entries"][el] = secret;
  }

  string json_data = data.dump ();
  out << data;
  out.close ();

  return status;
}

bridge_status_t
br_file_create (string filepath, string master_key)
{
  bridge_status_t status = BRIDGE_OK;
  ofstream out;
  json   data;

  out.open (filepath, ios::binary | ios::out);

  // set file json data
  data["version"] = SECPASS_FILE_VERSION;
  data["kdf"]     = KDF_ARGON2; // only supported atm
  data["enc"]     = ENC_AES128GCM;

  string json_data = data.dump ();

  out << json_data;
  out.close ();

  return status;
}

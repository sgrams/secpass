/* src/core/core.cc
 * Stanisław Grams <sjg@fmdx.pl>
 *
 * Contains implementations of functions
 * accessing the enclave
 *
 * secpass.org
 * © 2019
 */
#include <sgx.h>
#include "core.h"
#include "core_t.h"

#include "random.h"
#include "crypto.h"

#include "string.h"

#include <map>
#include <cstring>
#include <string>

core_crypto_c crypto;
static std::map<string, uint8_t[MAX_SECRET_SIZ]> secret_map;
static key_256bit_t mem_key;

core_status_t
get_key (uint8_t *key, size_t key_len)
{
  core_status_t status = CORE_OK;
  memcpy (key, mem_key, key_len);

  return status;
}
core_status_t
get_iv (const char *name, uint8_t *iv, size_t *len)
{
  core_status_t status = CORE_OK;
  string str = string (name);

  memcpy (iv, secret_map[str], CRYPTO_MEM_IV_SIZE);

  return status;
}

core_status_t
get_salt (uint8_t *salt, size_t len)
{
  core_status_t status = CORE_OK;
  if (!salt || len <= 0) {
    status = CORE_ER_WR_PARAM;
    return status;
  }

  crypto.generate_iv (salt, len);
  return status;
}

core_status_t
create_iv (uint8_t *iv, size_t len)
{
  core_status_t status = CORE_OK;
  if (!len || !iv) {
    status = CORE_ER_WR_PARAM;
    return status;
  }

  status = crypto.generate_iv (iv, len);
  return status;
}

// authorization
core_status_t
auth (uint8_t *mem_key_ptr)
{
  core_status_t status = CORE_OK;
  memcpy (mem_key, mem_key_ptr, CRYPTO_FILE_KEY_SIZE);

  return status;
}

core_status_t
deauth (void)
{
  memset (mem_key, 0, CRYPTO_FILE_KEY_SIZE);
  return CORE_OK;
}

// entries
core_status_t
entry_check (const char *name, uint8_t *rv)
{
  string name_str = string (name);
  if (!rv) {
    return CORE_ER_WR_PARAM;
  }
  *rv = !secret_map.count (name_str);

  return CORE_OK;
}

core_status_t
entry_add (const char *name, const char *secret)
{
  core_status_t status = CORE_OK;
  uint8_t rv;

  entry_check (name, &rv);
  if (rv) {
    uint8_t *ptr_iv  = secret_map[name];
    uint8_t *ptr_mac = ptr_iv  + CRYPTO_MEM_IV_SIZE;
    uint8_t *ptr_ct  = ptr_mac + CRYPTO_MEM_TAG_SIZE;

    status = crypto.generate_iv (ptr_iv, CRYPTO_MEM_IV_SIZE);
    if (0 != status) {
      return status;
    }

    status = crypto.encrypt_aes128gcm (
      mem_key, // key
      (uint8_t *)secret, // data_in
      MAX_SECRET_LEN, // ct len
      ptr_ct, // ct
      ptr_iv, // iv
      CRYPTO_MEM_IV_SIZE, // iv len
      (sgx_aes_gcm_128bit_tag_t *)ptr_mac// tag
      );
  }

  return status;
}

core_status_t
entry_del (const char *name)
{
  core_status_t status = CORE_OK;
  string name_str = string (name);

  // erase by mem_key
  secret_map.erase (name_str);

  return status;
}

//
// fetching secret
//
core_status_t
fetch_encrypted_secret (const char *name, uint8_t *secret)
{
  core_status_t status = CORE_OK;
  string str = string (name);

  memcpy (secret, secret_map[str], MAX_SECRET_SIZ);

  return status;
}

core_status_t
fetch_decrypted_secret (const char *name, char *secret)
{
  core_status_t status = CORE_OK;
  char          ret[MAX_SECRET_LEN] = {0};

  uint8_t *ptr_iv  = secret_map[name];
  uint8_t *ptr_mac = ptr_iv  + CRYPTO_MEM_IV_SIZE;
  uint8_t *ptr_ct  = ptr_mac + CRYPTO_MEM_TAG_SIZE;

  status = crypto.decrypt_aes128gcm (
    mem_key, // key
    ptr_ct,
    MAX_SECRET_LEN, // ct len
    (uint8_t *)ret,
    ptr_iv, // iv
    CRYPTO_MEM_IV_SIZE, // iv len
    (sgx_aes_gcm_128bit_tag_t *)ptr_mac // tag
    );

  memcpy (secret, ret, MAX_SECRET_LEN);

  return status;
}

//
// loading from file
//
core_status_t
load_to_sec_map (const char *name, const char *secret)
{
  core_status_t status = CORE_OK;
  string str_name = string (name);

  memcpy (secret_map[str_name], secret, MAX_SECRET_SIZ);

  return status;
}

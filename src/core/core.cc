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
#include <string>

core_crypto_c crypto;

static std::map<string, uint8_t[MAX_SECRET_LEN]> secret_map;
static std::map<string, iv_t>   iv_map;

static key_128bit_t mem_key;
uint8_t      tag[16];

core_status_t
get_iv (const char *name, uint8_t *iv, size_t *len)
{
  core_status_t status = CORE_OK;
  string str = string (name);

  memcpy (iv, iv_map[str], CRYPTO_MEM_IV_SIZE);

  return status;
}

core_status_t
create_iv (uint8_t *iv, size_t *len)
{
  core_status_t status = CORE_OK;
  if (!len || !iv) {
    status = CORE_ER_WR_PARAM;
    return status;
  }
  *len = CRYPTO_MEM_IV_SIZE;
  status = crypto.generate_iv (iv, *len);
  return status;
}

core_status_t
auth (uint8_t *mem_key_ptr)
{
  core_status_t status = CORE_OK;
  memcpy (mem_key, mem_key_ptr, CRYPTO_MEM_KEY_SIZE);

  return status;
}

core_status_t
deauth (void)
{
  memset (mem_key, 0, 16);
  return CORE_OK;
}

core_status_t
entry_check (const char *name, uint8_t *rv)
{
  string name_str = string (name);
  if (!rv) {
    return CORE_ER_WR_PARAM;
  }
  *rv = !secret_map.count (name_str) | !iv_map.count (name_str);

  return CORE_OK;
}

core_status_t
entry_add (const char *name, const char *secret)
{
  uint8_t rv;
  core_status_t status = CORE_OK;
  iv_t iv;

  entry_check (name, &rv);
  if (rv) {
    crypto.generate_iv (iv, CRYPTO_MEM_IV_SIZE);
    memcpy (iv_map[name], iv, CRYPTO_MEM_IV_SIZE);

    crypto.encrypt_aes128gcm (mem_key, (uint8_t *)secret, sizeof (secret), secret_map[name], iv_map[name], 12, tag);


    return status;
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
  iv_map.erase (name_str);

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

  memcpy (secret, secret_map[str], MAX_SECRET_LEN);
  return status;
}

core_status_t
fetch_decrypted_secret (const char *name, char *secret)
{
  char res[MAX_SECRET_LEN];
  core_status_t status = CORE_OK;
  string name_str = string (name);
  uint8_t *ptr = secret_map[name_str];

  crypto.decrypt_aes128gcm (mem_key, (uint8_t *)ptr, strlen ((const char *)ptr), (uint8_t *)res, iv_map[name], 12, tag);

  memcpy (secret, res, strlen (res));

  return status;
}

//
// loading from file
//
core_status_t
load_to_sec_map (const char *name, const uint8_t *secret)
{
  core_status_t status = CORE_OK;
  string str_name = string (name);

  memcpy (secret_map[str_name], secret, MAX_SECRET_LEN);

  return status;
}

core_status_t
load_to_iv_map (const char *name, const uint8_t *iv)
{
  core_status_t status = CORE_OK;
  string str_name = string (name);
  memcpy (iv_map[str_name], iv, CRYPTO_MEM_IV_SIZE);

  return status;
}

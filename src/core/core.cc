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

std::map<string, string> secret_map;
key_128bit_t key;
iv_t         iv     = {0};
bool         iv_set = false;
uint8_t      tag[16];

core_status_t
set_iv (void)
{
  core_status_t status = CORE_OK;
  if (!iv_set) {
    iv_set = true;
    status = crypto.generate_iv (iv, 12); // 96-bit
  }
  return status;
}
core_status_t
auth (uint8_t *key_ptr, size_t len)
{
  memcpy (key, key_ptr, len);
  // TODO: differ with generation between enc_t
  return set_iv ();
}

core_status_t
deauth (void)
{
  memset (key, 0, 16);
  return CORE_OK;
}

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
  uint8_t rv;
  core_status_t status = CORE_OK;
  uint8_t *sec = (uint8_t *)malloc (strlen (secret) + 1);

  entry_check (name, &rv);
  if (sec && rv) {
    crypto.encrypt_aes128gcm (key, (uint8_t *)secret, strlen (secret) + 1, sec, iv, 12, tag);
    secret_map[name] = string ((char *)sec);
    return status;
  }

  free (sec);
  return status;
}

core_status_t
entry_del (const char *name)
{
  core_status_t status = CORE_OK;
  string name_str = string (name);

  // erase by key
  secret_map.erase (name_str);

  return status;
}

core_status_t
entry_find (const char *name, char *secret)
{
  uint8_t rv;
  core_status_t status = CORE_OK;
  string name_str = string (name);
  status = entry_check (name, &rv);
  if (status != CORE_OK) {
    return status;
  }

  // TODO: implement checking, etc
  return status;
}

core_status_t
map_dump (uint8_t *content, size_t *len)
{
  core_status_t status = CORE_OK;

  return status;
}

core_status_t
map_load (uint8_t *content, size_t *len)
{
  core_status_t status = CORE_OK;

  return status;
}

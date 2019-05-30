/* src/core/core.cc
 * Stanisław Grams <sjg@fmdx.pl>
 *
 * Contains implementations of functions
 * accessing the enclave
 *
 * secpass.org
 * © 2019
 */
#include "core.h"

std::map<std::string, secret_t>           secret_map;
std::map<std::string, secret_t>::iterator secret_iter;

core_status_t
entry_add (
    uint64_t id,
    uint64_t expiry_time,

    char *name,
    size_t name_len,

    char *secret,
    size_t secret_len
    )
{
  core_status_t status = CORE_OK;
  secret_t secret;

  if (!name || !username || !password || !url || !notes ||
      name_len == 0 || username_len == 0 || password_len == 0 || notes_len == 0) {
    status = CORE_ER_WR_PARAM;
    return status;
  }

  secret =

  if (0 != secret_map.count (name)) {
    // key does not exist in the map
    status = CORE_ER_WR_PARAM;
    return status;
  }
  // TODO: !encrypt whole secret after parsing it!
  secret_map[name] = secret;

  if (1 != secret_map.count (name)) {
    status = CORE_ER_UNDEF;
  }

  return status;
}

core_status_t
entry_remove (char *name, size_t name_len)
{
  core_status_t status = CORE_OK;
  if (!name || name_len == 0) {
    status = CORE_ER_WR_PARAM;
    return status;
  }
  string str_name = string (name);

  if (1 != secret_map.count (str_name)) {
    status = CORE_ER_WR_PARAM;
    return status;
  }
  secret_map.erase (str_name);
  if (0 != secret_map.count (str_name)) {
    status = CORE_ER_UNDEF;
    return status;
  }

  return status;
}

core_status_t
entry_secret_encrypt (secret_t *secret)
{
  core_status_t status = CORE_OK;

  if (nullptr == secret) {
    status = CORE_ER_WR_PARAM;
    return status;
  }

  string to_enc = *secret;

  // perform encryption on the buffer
  encrypt_aes128gcm ()
}

core_status_t
entry_secret_decrypt (secret_t *secret)
{
  core_status_t status = CORE_OK;

  if (nullptr == secret) {
    status = CORE_ER_WR_PARAM;
    return status;
  }

}

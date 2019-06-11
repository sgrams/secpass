/* src/common/bridge.h
 * Stanisław Grams <sjg@fmdx.pl>
 *
 * secpass.org
 * 2019
 */
#ifndef _SECPASS_COMMON_BRIDGE_H
#define _SECPASS_COMMON_BRIDGE_H
#include <cstdint>
#include <string>
#include <vector>
#include <memory>

#include <openssl/sha.h>
#include <openssl/conf.h>
#include <openssl/evp.h>
#include <openssl/err.h>

#include "../core/core_u.h"
#include "../core/core.h"
#include "aes.h"

#include "sgx_eid.h"
#include "sgx_urts.h"

// version_t: variable determining secpass file format
typedef uint8_t version_t;

// kdf_t: variable determining KDF function used
typedef uint8_t kdf_t;
#define KDF_ARGON2    0x00
#define KDF_RESERVED  0xFF

// enc_t: enum determining encryption function used
typedef uint8_t enc_t;
#define ENC_AES128GCM 0x00
#define ENC_AES256CBC 0x01

#define ENC_RESERVED  0xFF

// bridge_status_t: 64 bit variable containing error code
typedef uint64_t bridge_status_t;

// definitions of error codes for core_status_t
#define BRIDGE_OK            0x00
#define BRIDGE_ER_INIT       0x01
#define BRIDGE_ER_DESTROY    0x02
#define BRIDGE_ER_ADD        0x03
#define BRIDGE_ER_DEL        0x04
#define BRIDGE_ER_FETCH      0x05
#define BRIDGE_ER_WR_PARAM   0x06
#define BRIDGE_ER_WR_FILE    0x07
#define BRIDGE_ER_WR_EXT     0x08
#define BRIDGE_ER_FILE_INIT  0x09
#define BRIDGE_ER_FILE_CLOSE 0x0A
#define BRIDGE_ER_NOMEM      0x0B
#define BRIDGE_ER_FILE_NONEX 0x0C
#define BRIDGE_ER_UNDEF      0xFF

// definitions of hash sizes and salt sizes
#define KDF_SALT_SIZE 8  // 64 bit
#define KDF_KEY_SIZE  32 // 256 bit
#define KDF_KEYFILE_HASH_SIZE 32 // 256 bit

bridge_status_t
br_enclave_init (void);

bridge_status_t
br_enclave_close (void);


bridge_status_t
br_derive_key_argon2 (
  const char *password, // IN
  const char *filepath, // IN
  uint8_t    *salt,     // OUT
  size_t     salt_len,  // IN
  uint8_t    *key,      // OUT
  size_t     key_len    // IN
  );

// file specific functions
bridge_status_t
br_file_get_init (string filepath, bool file_new, uint8_t *salt, size_t salt_len);

bridge_status_t
br_file_exists (string filepath);

bridge_status_t
br_file_valid (string filepath);

bridge_status_t
br_file_open (uint8_t *master_key, size_t master_key_len, string filepath);

bridge_status_t
br_file_save (string filepath, uint8_t *salt, size_t salt_len);

// secret specific functions
bridge_status_t
br_secret_add (string name, string secret);

bridge_status_t
br_secret_set (char *name, size_t name_len, char *secret, size_t secret_len);

bridge_status_t
br_secret_del (string name);

bridge_status_t
br_secret_check (string name, uint8_t *rv);

bridge_status_t
br_secret_fetch (string name, char *secret);

bridge_status_t
br_fetch_names (vector<string> *rv);

// authorization (sending master key to enclave)
bridge_status_t
br_auth (uint8_t *master_key, size_t master_key_len);

void
aes_256_cbc_encrypt (
  uint8_t *key,
  uint8_t *iv,
  const string &ptext,
  string& ctext
  );

void
aes_256_cbc_decrypt (
  uint8_t *key,
  uint8_t *iv,
  const string &ctext,
  string& ptext
  );
#endif // _SECPASS_COMMON_BRIDGE_H

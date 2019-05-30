/* src/core/core.h
 * Stanisław Grams <sjg@fmdx.pl>
 *
 * Contains definitions of functions
 * accessing the enclave
 *
 * secpass.org
 * © 2019
 */
#ifndef _SECPASS_CORE_H
#define _SECPASS_CORE_H
#include "../common/macros.h"
#include "../common/secpass.h"

#include <sgx.h>
// version of file format
#define SECPASS_CORE_FILE_VERSION 0x01

// definitions of error codes for core_status_t

#define CORE_OK          0x00
#define CORE_ER_WR_PARAM 0x01
#define CORE_ER_DECRYPT  0x02
#define CORE_ER_ENCRYPT  0x03

#define CORE_ER_DRNG     0x04
#define CORE_ER_UNDEF    0xFF

// core_status_t: 64 bit variable containing error code
typedef uint64_t core_status_t;
// version_t: 8 bit variable determining version type
typedef uint8_t version_t;

// kdf_t: enum determining KDF function used
typedef enum kdf { ARGON2, RESERVED } kdf_t;

// enc_t: enum determining encryption function used
typedef enum enc { AES256GCM, RESERVED } enc_t;

// iv_t: 8 bit variable containing init vector
typedef uint8_t iv_t[32]; // 32 * 8 bit = 256 bit IV

// file_t: structure containing both secret data and its' metadata
typedef struct file {
  version_t  version;
  kdf_t      kdf;
  iv_t       iv;
  enc_t      enc;

  database_t  db;
  string      db_path;
  string      db_key;
} file_t;

// database_t: structure-wrapper containing informations about entities
typedef struct database {
  uint64_t epoch_created;
  uint64_t epoch_modified;
  std::vector<entity_t> entities;
} database_t;

// entity_t: structure-wrapper for secret data and it's metadata
typedef struct entity {
  uint64_t id;

  uint64_t expiry_time; // expiry time epoch
  uint8_t  expired;

  string   name;
  secret_t secret;
} entity_t;

typedef string secret_t;

// ECALL prototypes
// !USER LEVEL (WRAPPERS TO GUI) SHALL BE PROVIDED HERE!
core_status_t
entry_add (
    char *name,
    size_t name_len,
    char *secret,
    size_t secret_len,
    );

// removes an entry by given name
core_status_t
entry_remove (char *name, size_t name_len);

// decrypts a secret given in a parameter
core_status_t
entry_secret_decrypt (secret_t *secret);

// encrypts a secret given in a parameter
core_status_t
entry_secret_encrypt (secret_t *secret);

// decrypts db_key given in a parameter
core_status_t
database_key_decrypt (string *key, );

// returns an entity or NULL in *entity parameter
core_status_t
entry_find (string name, entity_t *entity);

// returns a bool if an entry has expired
core_status_t
entry_check_if_expired (char *name, bool *expired, size_t name_len);

// OCALL prototypes
// gets epoch to given uint64_t pointer
core_status_t
ocall_get_epoch (uint64_t *time);

#endif // _SECPASS_CORE_H

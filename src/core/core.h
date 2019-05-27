/* src/core/core.h
 * Stanisław Grams <sgrams@fmdx.pl>
 *
 * Contains definitions of functions
 * accessing the enclave
 *
 * secpass.org
 * © 2019
 */
#ifndef _SECPASS_CORE_H
#define _SECPASS_CORE_H
#include <sgx.h>
// version of file format
#define SECPASS_CORE_FILE_VERSION 0x01

// core_status_t: 64 bit variable containing error code
typedef uint64_t core_status_t;
// version_t: 8 bit variable determining version type
typedef uint8_t version_t;

// kdf_t: enum determining KDF function used
typedef enum kdf { ARGON2, RESERVED } kdf_t;

// enc_t: enum determining encryption function used
typedef enum enc { AES256CBC, RESERVED } enc_t;

// iv_t: 8 bit variable containing init vector
typedef uint8_t iv_t[32]; // 32 * 8 bit = 256 bit IV

// file_t: structure containing both secret data and its' metadata
typedef struct file {
  version_t  version;
  kdf_t      kdf;
  iv_t       iv;
  enc_t      enc;
  // encrypted with determined by enc_t
  database_t *database;
} file_t;

// database_t: structure-wrapper containing informations about entities
typedef struct database {
  uint64_t epoch_created;
  uint64_t epoch_modified;
  entity_t *entities;
  size_t   entities_len;
} database_t;

// entity_t: structure-wrapper for secret data and it's metadata
typedef struct entity {
  uint64_t id;

  uint64_t expiry_time; // expiry time epoch
  uint8_t  expired;

  char     *name;
  size_t   name_len;
  secret_t secret;
} entity_t;

// secret_t: structure containing secret data
typedef struct secret {
  char   *username;
  size_t username_len;
  char   *password;
  size_t password_len;
  char   *url;
  size_t url_len;
  char   *notes;
  size_t notes_len;
} secret_t;


// ECALL prototypes
core_status_t
entry_add (
    char *name,
    size_t name_len,
    char *username,
    size_t username_len,
    char *url,
    size_t url_len,
    char *notes,
    size_t notes_len);

core_status_t
entry_remove (char *name, size_t name_len);

core_status_t
entry_secret_decrypt (secret_t *secret);

core_status_t
entry_secret_encrypt (secret_t *secret);

core_status_t
entry_find (string name, entity_t *entity);

// OCALL prototypes
core_status_t
entry_check_if_expired (char *name, size_t name_len);

#endif // _SECPASS_CORE_H

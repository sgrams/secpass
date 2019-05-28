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
#include "../common/macros.h"
#include "../common/secpass.h"

// version of file format
#define SECPASS_CORE_FILE_VERSION 0x01

// definitions of error codes for core_status_t

#define CORE_OK          0x00
#define CORE_ER_WR_PARAM 0x01
#define CORE_ER_DECRYPT  0x02
#define CORE_ER_ENCRYPT  0x03
// ...
#define CORE_ER_UNDEF    0xFF

// core_status_t: 64 bit variable containing error code
typedef uint64_t core_status_t;
// version_t: 8 bit variable determining version type
typedef uint8_t version_t;

// kdf_t: enum determining KDF function used
typedef enum kdf { ARGON2, RESERVED } kdf_t;

// enc_t: enum determining encryption function used
typedef enum enc { AES256GCM, RESERVED } enc_t;

// nonce_t: 8 bit variable containing init vector
typedef uint8_t nonce_t[32]; // 32 * 8 bit = 256 bit IV

// file_t: structure containing both secret data and its' metadata
typedef struct file {
  version_t  version;
  kdf_t      kdf;
  nonce_t    nonce;
  enc_t      enc;
  // encrypted with determined by enc_t
  string path;
  database_t *database;
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

// secret_t: structure containing secret data
typedef struct secret {
  string username;
  string password;
  string url;
  string notes;
} secret_t;


// ECALL prototypes
core_status_t
entry_add (
    char *name,
    size_t name_len,
    char *username,
    size_t username_len,
    char *password,
    size_t password_len,
    char *url,
    size_t url_len,
    char *notes,
    size_t notes_len);

// removes an entry by given name
core_status_t
entry_remove (char *name, size_t name_len);

// decrypts a secret given in a parameter
core_status_t
entry_secret_decrypt (secret_t *secret);

// encrypts a secret given in a parameter
core_status_t
entry_secret_encrypt (secret_t *secret);

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

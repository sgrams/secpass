/* src/common/wrapper.h
 * Stanisław Grams <sjg@fmdx.pl>
 *
 * secpass.org
 * 2019
 */
#ifndef _SECPASS_COMMON_WRAPPER_H
#define _SECPASS_COMMON_WRAPPER_H
#include <cstdint>
#include <string>
#include "../core/core_u.h"
#include "../core/core.h"

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
#define ENC_RESERVED  0xFF

// wrapper_status_t: 64 bit variable containing error code
typedef uint64_t wrapper_status_t;

// definitions of error codes for core_status_t
#define WRAP_OK          0x00
#define WRAP_ER_INIT     0x01
#define WRAP_ER_DESTROY  0x02
#define WRAP_ER_ADD      0x03
#define WRAP_ER_DEL      0x04
#define WRAP_ER_WR_PARAM 0x05
#define WRAP_ER_UNDEF    0xFF

wrapper_status_t
initialize_enclave (void);

wrapper_status_t
destroy_enclave (void);

wrapper_status_t
file_auth   (string key, string filepath);

wrapper_status_t
file_deauth (void);

wrapper_status_t
secret_add (string name, string secret);

wrapper_status_t
secret_set (char *name, size_t name_len, char *secret, size_t secret_len);

wrapper_status_t
secret_del (string name);

wrapper_status_t
secret_check (string name, uint8_t *rv);
//wrapper_status_t
//secret_list (std::vector<std::string> *vector);

#endif // _SECPASS_COMMON_WRAPPER_H

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

using namespace std;
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
typedef enum kdf { ARGON2, KDF_RESERVED } kdf_t;

// enc_t: enum determining encryption function used
typedef enum enc { AES256GCM, ENC_RESERVED } enc_t;

// iv_t: 8 bit variable containing init vector
typedef uint8_t iv_t[32]; // 32 * 8 bit = 256 bit IV

typedef char * secret_t;
#endif // _SECPASS_CORE_H

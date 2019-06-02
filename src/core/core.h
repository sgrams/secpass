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
#include <sgx.h>
#include <string>

#include "../common/macros.h"
#include "../common/secpass.h"

using namespace std;

// core_status_t: 64 bit variable containing error code
typedef uint64_t core_status_t;
// definitions of error codes for core_status_t

#define CORE_OK          0x00
#define CORE_ER_WR_PARAM 0x01
#define CORE_ER_DECRYPT  0x02
#define CORE_ER_ENCRYPT  0x03

#define CORE_ER_DRNG     0x04
#define CORE_ER_CMAC     0x05
#define CORE_ER_UNDEF    0xFF

// iv_t: 8 bit variable containing init vector
typedef uint8_t iv_t[12]; // 12 * 8 bit = 96 bit IV

// secret_t: string containing the secret
typedef string secret_t;

// key_128_bit_t: 128 bit variable containing the master key
typedef uint8_t key_128bit_t[16];

#endif // _SECPASS_CORE_H

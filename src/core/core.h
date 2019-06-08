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

#endif // _SECPASS_CORE_H

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

#include "sgx_eid.h"
#include "sgx_urts.h"

int initialize_enclave (void);
int destroy_enclave (void);

int secret_add (char *name, size_t name_len, char *secret, size_t secret_len);
int secret_set (char *name, size_t name_len, char *secret, size_t secret_len);
int secret_del (char *name, size_t name_len);

#endif // _SECPASS_COMMON_WRAPPER_H

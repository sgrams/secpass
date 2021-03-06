/* src/core/crypto.h
 * Stanisław Grams <sjg@fmdx.pl>
 *
 * secpass.org
 * © 2019
 */
#ifndef _SECPASS_CORE_CRYPTO_H
#define _SECPASS_CORE_CRYPTO_H
#include "core.h"
#include <sgx.h>
#include "sgx_tcrypto.h"

#define SECPASS_CRYPTO_VERSION 0x001

#define CRYPTO_MEM_IV_SIZE   12 // 96 bit
#define CRYPTO_MEM_KEY_SIZE  16 // 128 bit
#define CRYPTO_FILE_IV_SIZE  16 // 128 bit
#define CRYPTO_FILE_KEY_SIZE 32 // 256 bit

#define CRYPTO_MEM_TAG_SIZE  SGX_AESGCM_MAC_SIZE // 128 bit

// Error definitions
class core_crypto_c {
  public:
  // constructors and destructors
  core_crypto_c ();
  ~core_crypto_c ();

  core_status_t
  decrypt_aes128gcm (
    uint8_t  *key, // 128 bit key
    uint8_t  *in_data,
    uint32_t data_len,
    uint8_t  *out_data,
    uint8_t  *iv, // 96 bit init vector
    size_t   iv_len,
    sgx_aes_gcm_128bit_tag_t *tag
  );

  core_status_t
  encrypt_aes128gcm (
    uint8_t  *key, // 128 bit key
    uint8_t  *in_data,
    uint32_t data_len,
    uint8_t  *out_data,
    uint8_t  *iv, // 96 bit init vector
    size_t   iv_len,
    sgx_aes_gcm_128bit_tag_t *tag
  );

  core_status_t
  generate_iv (void *iv, size_t n);
};
#endif // _SECPASS_CORE_CRYPTO_H

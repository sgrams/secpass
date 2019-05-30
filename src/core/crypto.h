/* src/core/crypto.h
 * Stanisław Grams <sjg@fmdx.pl>
 *
 * secpass.org
 * © 2019
 */
#ifndef _SECPASS_CORE_CRYPTO_H
#define _SECPASS_CORE_CRYPTO_H
#include "core.h"

#define SECPASS_CRYPTO_VERSION 0x001
#define CRYPTO_IV_SIZE  12
#define CRYPTO_KEY_SIZE 16
// Error definitions
class Crypto {
  private:
  core_status_t
  decrypt_aes128gcm (
    uint8_t *key, // 128 bit key
    uint8_t *iv, // 96 bit init vector
    size_t iv_len,

    uint8_t *in_data,
    size_t in_data_len,
    uint8_t *out_data,
    size_t out_data_len
  );

  core_status_t
  encrypt_aes128gcm (
    uint8_t *key, // 128 bit key
    uint8_t *iv, // 96 bit init vector
    size_t iv_len,

    uint8_t *in_data,
    size_t in_data_len,
    uint8_t *out_data,
    size_t out_data_len
  );

  public:
  // constructors and destructors
  Crypto ();
  ~Crypto ();
};
#endif // _SECPASS_CORE_CRYPTO_H

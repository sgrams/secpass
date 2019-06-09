/* src/core/crypto.cc
 * Stanisław Grams <sjg@fmdx.pl>
 *
 * secpass.org
 * © 2019
 */
#include <sgx.h>
#include <sgx_tcrypto.h>

#include "crypto.h"
#include "random.h"

core_random_c random;

core_crypto_c::core_crypto_c ()
{
}

core_crypto_c::~core_crypto_c ()
{
}

core_status_t
core_crypto_c::decrypt_aes128gcm (
  uint8_t  *key, // 128 bit key
  uint8_t  *in_data,
  uint32_t data_len,
  uint8_t  *out_data,
  uint8_t  *iv, // 96 bit init vector
  size_t   iv_len,
  sgx_aes_gcm_128bit_tag_t *tag
  )
{
  if (SGX_SUCCESS != sgx_rijndael128GCM_decrypt ((sgx_aes_gcm_128bit_key_t *)key, in_data, (uint32_t)data_len, out_data, iv, (uint32_t)iv_len, NULL, 0, (sgx_aes_gcm_128bit_tag_t *)tag)) {
    return CORE_ER_DECRYPT;
  }
  return CORE_OK;
}

core_status_t
core_crypto_c::encrypt_aes128gcm (
  uint8_t  *key, // 128 bit key
  uint8_t  *in_data,
  uint32_t data_len,
  uint8_t  *out_data,
  uint8_t  *iv, // 96 bit init vector
  size_t   iv_len,
  sgx_aes_gcm_128bit_tag_t *tag
  )
{
  if (SGX_SUCCESS != sgx_rijndael128GCM_encrypt ((sgx_aes_gcm_128bit_key_t *)key, in_data, (uint32_t)data_len, out_data, iv, (uint32_t)iv_len, NULL, 0, (sgx_aes_gcm_128bit_tag_t *)tag)) {
    return CORE_ER_ENCRYPT;
  }
  return CORE_OK;
}

core_status_t
core_crypto_c::generate_iv (void *iv, size_t n)
{
  return random.get_n_bytes_rand (iv, n);
}

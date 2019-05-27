/* src/core/crypto.h
 * Stanisław Grams <sgrams@fmdx.pl>
 *
 * secpass.org
 * © 2019
 */
#ifndef _SECPASS_CORE_CRYPTO_H
#define _SECPASS_CORE_CRYPTO_H

#define SECPASS_CRYPTO_VERSION 0x001

// crypto_status_t: 64 bit variable containing error code
typedef uint64_t crypto_status_t;

#define BIT(x) (1 << (x))

// Error definitions
#define ENCLAVE_OK           BIT(0)

#define ENCLAVE_ER_ENCRYPT   BIT(1)
#define ENCLAVE_ER_DECRYPT   BIT(2)

class Crypto {
  public:
  Crypto ();
  ~Crypto ();

};
#endif // _SECPASS_CORE_CRYPTO_H

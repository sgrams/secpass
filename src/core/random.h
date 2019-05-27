/* src/core/random.h
 * Stanisław Grams <sgrams@fmdx.pl>
 *
 * secpass.org
 * © 2019
 */
#ifndef _SECPASS_CORE_RANDOM_H
#define _SECPASS_CORE_RANDOM_H
#include <sgx.h>
#include <sgx_trts.h>
#include <sgx_cpuid.h>
#include <sgx_tcrypto.h>

// random_status_t: 64 bit variable containing error code
typedef uint64_t random_status_t;

class Random {
  random_status_t get_u64_rand (uint64_t *rand);
  random_status_t get_u64_seed (uint64_t *seed);

  public:
  Random ();
  ~Random ();

};
#endif // _SECPASS_CORE_RANDOM_H

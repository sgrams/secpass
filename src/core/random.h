/* src/core/random.h
 * Stanisław Grams <sjg@fmdx.pl>
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

#define DRNG_NOT_READY   0x00
#define DRNG_UNAVAILABLE 0x01
#define DRNG_AVAILABLE   0x02

#define DRNG_DEF_RETRIES 10

int32_t _get_rng_support (int32_t *cpuinfo);

class core_random_c {
  public:
  core_random_c ();
  ~core_random_c ();

  bool
  get_rng_support ();

  // random
  core_status_t
  get_u32_rand (uint32_t *rand);

  core_status_t
  get_u64_rand (uint64_t *rand);

  core_status_t
  get_n_bytes_rand (void *iv, size_t n);
  // seed
  core_status_t
  get_u32_seed (uint32_t *seed);

  core_status_t
  get_u64_seed (uint64_t *seed);

  core_status_t
  get_n_bytes_rdrand_seed (uint8_t n, void *seed);
};
#endif // _SECPASS_CORE_RANDOM_H

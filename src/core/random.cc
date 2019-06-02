/* src/core/random.cc
 * Stanisław Grams <sjg@fmdx.pl>
 *
 * secpass.org
 * © 2019
 */
#include <sgx.h>
#include <sgx_trts.h>
#include <sgx_cpuid.h>
#include <sgx_tcrypto.h>
#include <sgx_thread.h>
#include <sgx_dh.h>
#include <string.h>


#include "crypto.h"
#include "random.h"

static int _drng_init = DRNG_NOT_READY;

int32_t
_get_rng_support (int32_t *cpuinfo)
{
  int32_t ret = DRNG_UNAVAILABLE;

  #ifdef COMPILER_HAS_RDSEED_SUPPORT
	__cpuidex(info, 7, 0);
	if (((uint32_t) cpuinfo[1]) & (1<<18) ) ret |= DRNG_AVAILABLE;
#endif

  return ret;
}

Random::Random ()
{
  int32_t cpuinfo[4];
  sgx_status_t status;

  status = sgx_cpuidex (cpuinfo, 1, 0);
  if (SGX_SUCCESS == status) {
    _drng_init = _get_rng_support (cpuinfo);
  }
}

Random::~Random ()
{
}

bool
Random::get_rng_support ()
{
  return (_drng_init & DRNG_AVAILABLE) == DRNG_AVAILABLE;
}

core_status_t
Random::get_u32_rand (uint32_t *rand)
{
  core_status_t status = CORE_OK;
  uint32_t retries = DRNG_DEF_RETRIES;

  while (retries > 0)
  {
    if (sgx_read_rand ((uint8_t *)rand, 4) != SGX_SUCCESS) {
      status = CORE_ER_DRNG;
    }
    retries--;
  }

  return status;
}

core_status_t
Random::get_u64_rand (uint64_t *rand)
{
  core_status_t status = CORE_OK;
  uint32_t retries = DRNG_DEF_RETRIES;

  while (retries > 0)
  {
    if (sgx_read_rand ((uint8_t *)rand, 8) != SGX_SUCCESS) {
      status = CORE_ER_DRNG;
    }
    retries--;
  }

  return status;
}


core_status_t
Random::get_u32_seed (uint32_t *seed)
{
#ifdef COMPILER_HAS_RDSEED_SUPPORT
  core_status_t status = CORE_OK;
  uint32_t retries = DRNG_DEF_RETRIES;

  if (!get_rng_support ()) {
    return get_n_bytes_rdrand_seed (4, seed);
  }

  while (retries > 0)
  {
    if (_rdseed32_step (seed)) {
      return CORE_OK;
    }
    _mm_pause ();
    retries--;
  }

#else
  return get_n_bytes_rdrand_seed (4, seed);
#endif
}

core_status_t
Random::get_u64_seed (uint64_t *seed)
{
#ifdef COMPILER_HAS_RDSEED_SUPPORT
  core_status_t status = CORE_OK;
  uint32_t retries = DRNG_DEF_RETRIES;

  if (!get_rng_support ()) {
    return get_n_bytes_rdrand_seed (8, seed);
  }

  while (retries > 0)
  {
    if (_rdseed64_step (seed)) {
      return CORE_OK;
    }
    _mm_pause ();
    retries--;
  }

#else
  return get_n_bytes_rdrand_seed (8, seed);
#endif
}

core_status_t
Random::get_n_bytes_rdrand_seed (uint8_t n, void *seed) {
  // Use CMAC to generate 128-bit seeds from rdrand.
  // DRNG is guaranteed to reseed after 512 * 128 bits generated.
  core_status_t           status = CORE_OK;
  uint8_t                 key[16];
  uint8_t                 rand[16 * 512];
  uint8_t                 *ptr = (uint8_t *)seed;
  sgx_cmac_128bit_tag_t   hash;
  sgx_cmac_state_handle_t hcmac;

  if (SGX_SUCCESS != sgx_read_rand (key, 16)) {
    return CORE_ER_DRNG;
  }

  // set up key
  if (SGX_SUCCESS != sgx_cmac128_init ((const sgx_cmac_128bit_key_t *)key, &hcmac)) {
    return CORE_ER_CMAC;
  }

  while (n > 0) {
    uint64_t chunk = (n >= 16) ? 16 : n;
    if (SGX_SUCCESS != sgx_read_rand (rand, 16 * 512)) {
      status = CORE_ER_DRNG;
      goto GET_N_BYTES_RDRAND_SEED_CLEANUP;
    }
    if (SGX_SUCCESS != sgx_cmac128_update (rand, 16 * 512, hcmac)) {
      status = CORE_ER_CMAC;
      goto GET_N_BYTES_RDRAND_SEED_CLEANUP;
    }
    if (SGX_SUCCESS != sgx_cmac128_final (hcmac, &hash)) {
      status = CORE_ER_CMAC;
      goto GET_N_BYTES_RDRAND_SEED_CLEANUP;
    }
    memcpy (ptr, hash, chunk);
    n   = n - chunk;
    ptr = ptr + chunk;
  }

GET_N_BYTES_RDRAND_SEED_CLEANUP:
  sgx_cmac128_close (hcmac);
  return status;
}
